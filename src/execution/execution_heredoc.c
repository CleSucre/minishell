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

int	*g_exit_code;

static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\nminishell: heredoc interrupted by Ctrl+C\n",
			STDERR_FILENO);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
}

/**
 * @brief Handle the signal in the parent process
 * 			to set the exit code to 130 on SIGINT
 *
 * @param int sig
 */
void	parent_signal_handler(int sig)
{
	if (sig == SIGINT)
		*g_exit_code = 130;
}

/**
 * @brief Load the heredoc info
 *
 * @param t_minishell *minishell
 * @param int *pipes
 * @param char *delimiter
 * @return t_heredoc_info*
 */
t_heredoc_info	*load_heredoc_info(t_minishell *minishell,
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

void	heredoc_sa_flag_pid(void)
{
	struct sigaction	sa_child;

	ft_memset(&sa_child, 0, sizeof(sa_child));
	sa_child.sa_flags = 0;
	sa_child.sa_handler = heredoc_signal_handler;
	sigemptyset(&sa_child.sa_mask);
	sigaction(SIGINT, &sa_child, NULL);
}

/**
 * @brief Run the heredoc logic in a fork and handle signals in the child
 *
 * @param t_minishell *minishell
 * @param char *delimiter
 * @param int *output_fd
 * @return int 1 on success, 0 on failure
 */
int	run_heredoc(t_minishell *minishell, char *delimiter, int *output_fd)
{
	pid_t				pid;
	int					tmp_pipe[2];

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
	else if (pid == 0)
		work_pid(minishell, delimiter);
	else if (!last_option(output_fd, minishell, pid, tmp_pipe))
		return (0);
	return (1);
}
