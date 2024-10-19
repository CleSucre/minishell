/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 03:29:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/19 03:29:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the given input is valid before starting the heredoc
 *
 * @param int *in_out
 * @param int *pipes
 * @return int 1 on valid, 0 on invalid, -1 on failure
 */
int	heredoc_valid(t_ast_node *ast, int *pipes)
{
	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no delimiter specified\n", STDERR_FILENO);
		return (0);
	}
	if (pipe(pipes) == -1 && !ast->is_last)
	{
		ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
		return (-1);
	}
	return (1);
}

/**
 * @brief Write the heredoc content to the pipe
 *
 * @param t_minishell *minishell
 * @param int *pipes
 * @param int *in_out
 * @param char *line
 * @return int 1 on success, 0 on failure
 */
static int	write_heredoc(t_minishell *minishell, int *pipes,
							int *in_out, char *line)
{
	char	*tmp;

	tmp = replace_variables(minishell, line);
	free(line);
	if (write(pipes[1], tmp, ft_strlen(tmp)) == -1)
	{
		ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
		free(tmp);
		close_fds(in_out, pipes);
		return (1);
	}
	free(tmp);
	return (0);
}

/**
 * @brief Signal handler for heredoc
 *
 * @param int sig
 * @param siginfo_t *info
 * @param void *context
 */
static void	signal_handler(int sig, siginfo_t *info, void *context)
{
	t_heredoc_info	*heredoc_info;

	(void)sig;
	(void)context;
	heredoc_info = (t_heredoc_info *)info->si_value.sival_ptr;
	if (heredoc_info && heredoc_info->delimiter)
	{
		ft_fprintf(STDERR_FILENO,
			"\nminishell: heredoc terminated (wanted `%s`)\n",
			heredoc_info->delimiter);
		close(heredoc_info->pipes[0]);
		close(heredoc_info->pipes[1]);
	}
}

/**
 * @brief Run the heredoc logic
 *
 * @param t_minishell *minishell
 * @param char *delimiter
 * @param int *pipes
 * @param int *in_out
 * @return int 1 on success, 0 on failure
 */
int	run_heredoc(t_minishell *minishell, char *delimiter,
					int *pipes, int *in_out)
{
	char				*tmp;
	char				*line;
	int					i;
	struct sigaction	sa;
	union sigval		sig_data;
	t_heredoc_info		heredoc_info;

	(void)sig_data;
	heredoc_info.delimiter = delimiter;
	heredoc_info.pipes = pipes;
	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGINT, &sa, NULL);
	sig_data.sival_ptr = &heredoc_info;
	i = 1;
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_fprintf(STDERR_FILENO, "\nminishell: warning:"
				"here-document at line %d delimited by end-of-file"
				"(wanted `%s`)\n", i, delimiter);
			close_fds(in_out, pipes);
			free(line);
			return (1);
		}
		tmp = ft_strdup(line);
		tmp[ft_strlen(line) - 1] = '\0';
		if (!line || ft_strcmp(tmp, delimiter) == 0)
		{
			free(tmp);
			break ;
		}
		free(tmp);
		if (write_heredoc(minishell, pipes, in_out, line))
			return (0);
		i++;
	}
	free(line);
	in_out[0] = pipes[0];
	close(pipes[1]);
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	return (1);
}
