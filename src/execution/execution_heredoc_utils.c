/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_heredoc_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <pierrot.maxime.pro@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 01:18:56 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/24 01:18:56 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\nminishell: heredoc interrupted"
			"by Ctrl+C\n", STDERR_FILENO);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
}

void	setup_heredoc_parent_signals(void)
{
	struct sigaction	sa_parent;

	sigemptyset(&sa_parent.sa_mask);
	sa_parent.sa_flags = 0;
	sa_parent.sa_handler = parent_signal_handler;
	sigaction(SIGINT, &sa_parent, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	handle_heredoc_parent_process(t_minishell *minishell, int *tmp_pipe,
								pid_t pid, int *output_fd)
{
	int	status;

	setup_heredoc_parent_signals();
	close(tmp_pipe[1]);
	waitpid(pid, &status, 0);
	*output_fd = tmp_pipe[0];
	if (minishell->exit_code == 130)
	{
		close(*output_fd);
		return (0);
	}
	else
		minishell->exit_code = 0;
	return (1);
}

void	write_and_cleanup_heredoc(t_heredoc_info *heredoc_info, int fd)
{
	if (!write_heredoc(heredoc_info))
	{
		close(fd);
		ft_tabfree(heredoc_info->texts);
		free(heredoc_info);
		exit(1);
	}
	close(fd);
	ft_tabfree(heredoc_info->texts);
	free(heredoc_info);
	exit(0);
}
