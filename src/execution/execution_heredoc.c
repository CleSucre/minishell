/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 23:51:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/05 23:51:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "global.h"

/**
 * @brief Write the heredoc content to the pipe
 *
 * @param t_heredoc_info *heredoc_info Information sur le heredoc
 * @return int 1 on success, 0 on failure
 */
//
//int	write_line_to_pipe(int fd, const char *line)
//{
//	ssize_t	bites;
//
//	bites = write(fd, line, ft_strlen(line));
//	if (bites == -1)
//	{
//		ft_fprintf(STDERR_FILENO, "Error on fd %d: write failed\n", fd);
//		ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
//		return (0);
//	}
//	bites = write(fd, "\n", 1);
//	if (bites == -1)
//	{
//		ft_fprintf(STDERR_FILENO, "Error on fd %d: write failed\n", fd);
//		ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
//		return (0);
//	}
//	return (1);
//}
//
//int	write_heredoc(t_heredoc_info *heredoc_info)
//{
//	int	i;
//
//	i = 0;
//	while (heredoc_info->texts[i])
//	{
//		if (ft_strlen(heredoc_info->texts[i]) == 0)
//		{
//			i++;
//			continue ;
//		}
//		if (!write_line_to_pipe(heredoc_info->pipes[1],
//				heredoc_info->texts[i]))
//			return (0);
//		i++;
//	}
//	return (1);
//}

/**
 * @brief Load the heredoc info
 *
 * @param t_minishell *minishell
 * @param int *pipes
 * @param char *delimiter
 * @return t_heredoc_info*
 */
static t_heredoc_info	*load_heredoc_info(t_minishell *minishell,
											int *pipes, char *delimiter)
{
	t_heredoc_info	*heredoc_info;

	heredoc_info = malloc(sizeof(t_heredoc_info));
	if (!heredoc_info)
		return (NULL);
	heredoc_info->minishell = minishell;
	heredoc_info->delimiter = ft_strdup(delimiter);
	heredoc_info->pipes = pipes;
	heredoc_info->texts = ft_tabnew(0);
	if (!heredoc_info->texts)
	{
		free(heredoc_info->delimiter);
		free(heredoc_info);
		return (NULL);
	}
	return (heredoc_info);
}
//
//static char	*begin_read(void)
//{
//	char	*tmp;
//	char	*line;
//
//	ft_putstr_fd("> ", STDOUT_FILENO);
//	tmp = get_next_line(STDIN_FILENO);
//	line = ft_strtrim(tmp, "\n");
//	free(tmp);
//	return (line);
//}
//
//static int	read_heredoc(t_heredoc_info *heredoc_info)
//{
//	int		i;
//	char	*line;
//
//	i = 0;
//	while (1)
//	{
//		line = begin_read();
//		if (!line)
//		{
//			ft_fprintf(STDERR_FILENO, "minishell: warning: heredoc at line %d"
//				"delimited by end-of-file (wanted `%s`)\n",
//				i, heredoc_info->delimiter);
//			return (0);
//		}
//		if (ft_strcmp(line, heredoc_info->delimiter) == 0)
//		{
//			free(line);
//			break ;
//		}
//		ft_tabadd(&heredoc_info->texts, line);
//		free(line);
//		i++;
//	}
//	return (1);
//}

/**
 * @brief Run the heredoc logic in a fork and handle signals in the child
 *
 * @param t_minishell *minishell
 * @param char *delimiter
 * @param int *output_fd
 * @return int 1 on success, 0 on failure
 */

void	setup_heredoc_child_signals(void)
{
	struct sigaction	sa_child;

	ft_memset(&sa_child, 0, sizeof(sa_child));
	sa_child.sa_flags = 0;
	sa_child.sa_handler = heredoc_signal_handler;
	sigemptyset(&sa_child.sa_mask);
	sigaction(SIGINT, &sa_child, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_heredoc_child_process(t_minishell *minishell,
										int *tmp_pipe, char *delimiter)
{
	t_heredoc_info	*heredoc_info;

	heredoc_info = load_heredoc_info(minishell, tmp_pipe, delimiter);
	if (!heredoc_info)
		exit(1);
	setup_heredoc_child_signals();
	close(tmp_pipe[0]);
	read_heredoc(heredoc_info);
	free(heredoc_info->delimiter);
	close_all_fds(minishell->opened_fds);
	free_minishell(heredoc_info->minishell);
	write_and_cleanup_heredoc(heredoc_info, tmp_pipe[1]);
}

int	run_heredoc(t_minishell *minishell, char *delimiter, int *output_fd)
{
	pid_t	pid;
	int		tmp_pipe[2];

	if (pipe(tmp_pipe) == -1)
		return (0);
	g_exit_code = &minishell->exit_code;
	pid = fork();
	if (pid < 0)
	{
		close(tmp_pipe[0]);
		close(tmp_pipe[1]);
		return (0);
	}
	if (pid == 0)
		handle_heredoc_child_process(minishell, tmp_pipe, delimiter);
	return (handle_heredoc_parent_process(minishell, tmp_pipe, pid, output_fd));
}
