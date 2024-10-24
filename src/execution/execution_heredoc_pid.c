/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_heredoc_pid.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <pierrot.maxime.pro@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 01:22:26 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/24 01:22:26 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	work_pid(t_minishell *minishell, char *delimiter)
{
	t_heredoc_info		*heredoc_info;
	int					tmp_pipe[2];

	heredoc_info = load_heredoc_info(minishell, tmp_pipe, delimiter);
	if (!heredoc_info)
		exit(1);
	heredoc_sa_flag_pid();
	signal(SIGQUIT, SIG_IGN);
	close(tmp_pipe[0]);
	read_heredoc(heredoc_info);
	free(heredoc_info->delimiter);
	close_all_fds(minishell->opened_fds);
	free_minishell(heredoc_info->minishell);
	if (!write_heredoc(heredoc_info))
	{
		close(tmp_pipe[1]);
		ft_tabfree(heredoc_info->texts);
		free(heredoc_info);
		exit(1);
	}
	close(tmp_pipe[1]);
	ft_tabfree(heredoc_info->texts);
	free(heredoc_info);
	exit(0);
}

int	last_option(int *output_fd, t_minishell *minishell,
		pid_t pid, int *tmp_pipe)
{
	struct sigaction	sa_parent;
	int					status;

	status = 0;
	sigemptyset(&sa_parent.sa_mask);
	sa_parent.sa_flags = 0;
	sa_parent.sa_handler = parent_signal_handler;
	sigaction(SIGINT, &sa_parent, NULL);
	signal(SIGQUIT, SIG_IGN);
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
