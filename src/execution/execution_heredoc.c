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
static int	write_heredoc(t_heredoc_info *heredoc_info)
{
	char	*tmp;

	if (heredoc_info->expend_var)
		tmp = replace_variables(heredoc_info->minishell, heredoc_info->line);
	else
		tmp = ft_strdup(heredoc_info->line);
	free(heredoc_info->line);
	if (write(heredoc_info->pipes[1], tmp, ft_strlen(tmp)) == -1)
	{
		ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
		free(tmp);
		close_fds(heredoc_info->in_out, heredoc_info->pipes);
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
 * @brief Load the heredoc info
 *
 * @param int *pipes
 * @param int *in_out
 * @param char *delimiter
 * @return t_heredoc_info*
 */
static t_heredoc_info	*load_heredoc_info(
		t_minishell *minishell, int *pipes, int *in_out, char *delimiter)
{
	char			*new_delimiter;
	size_t 			old_len;
	t_heredoc_info	*heredoc_info;

	heredoc_info = malloc(sizeof(t_heredoc_info));
	if (!heredoc_info)
		return (NULL);
	old_len = ft_strlen(delimiter);
	new_delimiter = parse_quotes(delimiter);
	delimiter = new_delimiter;
	if (!*delimiter)
	{
		free(heredoc_info);
		return (NULL);
	}
	if (ft_strlen(delimiter) != old_len)
		heredoc_info->expend_var = 0;
	else
		heredoc_info->expend_var = 1;
	heredoc_info->minishell = minishell;
	heredoc_info->delimiter = delimiter;
	heredoc_info->pipes = pipes;
	heredoc_info->in_out = in_out;
	heredoc_info->line = NULL;
	return (heredoc_info);
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
	int					i;
	struct sigaction	sa;
	union sigval		sig_data;
	t_heredoc_info		*heredoc_info;

	heredoc_info = load_heredoc_info(minishell, pipes, in_out, delimiter);
	if (!heredoc_info)
		return (0);
	(void)sig_data;
	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sig_data.sival_ptr = heredoc_info;

	i = 1;
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		heredoc_info->line = get_next_line(STDIN_FILENO);
		if (!heredoc_info->line)
		{
			ft_fprintf(STDERR_FILENO, "\nminishell: warning: "
									  "here-document at line %d delimited by end-of-file "
									  "(wanted `%s`)\n", i, heredoc_info->delimiter);
			close_fds(in_out, pipes);
			free(heredoc_info->line);
			return (1);
		}
		tmp = ft_strdup(heredoc_info->line);
		tmp[ft_strlen(heredoc_info->line) - 1] = '\0';
		if (ft_strcmp(tmp, heredoc_info->delimiter) == 0)
		{
			free(tmp);
			break ;
		}
		free(tmp);
		if (write_heredoc(heredoc_info))
			return (0);
		i++;
	}
	free(heredoc_info->line);
	free(heredoc_info->delimiter);
	in_out[0] = pipes[0];
	close(pipes[1]);

	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	return (1);
}
