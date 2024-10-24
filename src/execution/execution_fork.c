/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 23:51:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/05 23:51:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		signal(SIGINT, SIG_DFL);
	}
	else if (sig == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
		signal(SIGQUIT, SIG_DFL);
	}
}

void	setup_signals(struct sigaction *sa)
{
	sa->sa_handler = handle_signal;
	sa->sa_flags = 0;
	sigemptyset(&sa->sa_mask);
	if (sigaction(SIGINT, sa, NULL) == -1 || sigaction(SIGQUIT, sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

static void	handle_child_process(t_cmd *cmd, t_minishell *minishell)
{
	int	err;

	if (cmd->input_fd != STDIN_FILENO)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}
	if (cmd->output_fd != STDOUT_FILENO)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
	if (cmd->to_close != -1)
		close(cmd->to_close);
	close_all_fds(minishell->opened_fds);
	err = execute_path(cmd);
	destroy_cmd(cmd);
	free_minishell(minishell);
	exit(err);
}

static void	handle_builtins_child_process(t_cmd *cmd, t_minishell *minishell)
{
	int	err;

	err = execute_builtin_command(minishell, cmd);
	if (cmd->input_fd != STDIN_FILENO)
	{
		dup2(cmd->input_fd, STDIN_FILENO);
		close(cmd->input_fd);
	}
	if (cmd->output_fd != STDOUT_FILENO)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
	if (cmd->to_close != -1)
		close(cmd->to_close);
	destroy_cmd(cmd);
	close_all_fds(minishell->opened_fds);
	free_minishell(minishell);
	exit(err);
}

int	execute_external(t_minishell *minishell, t_cmd *cmd)
{
	struct sigaction	sa;
	pid_t				pid;

	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("Error: fork failed\n", STDERR_FILENO);
		destroy_cmd(cmd);
		return (1);
	}
	else if (pid == 0)
	{
		setup_signals(&sa);
		if (is_builtin_command(cmd))
			handle_builtins_child_process(cmd, minishell);
		else
			handle_child_process(cmd, minishell);
	}
	else
		setup_signals(&sa);
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	return (pid);
}
