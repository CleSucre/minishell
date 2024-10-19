/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:13:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/23 12:00:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Set up signal handling for SIGUSR1
 *
 * @param struct sigaction *sa Signal action structure
 */
static void	setup_signals(struct sigaction *sa)
{
	sa->sa_handler = handle_signal;
	sa->sa_flags = 0;
	sigemptyset(&sa->sa_mask);
	if (sigaction(SIGUSR1, sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Handle the child process after fork
 *
 * @param t_cmd *cmd Command structure
 * @param t_minishell *minishell Minishell context
 * @param struct sigaction *sa Signal action structure
 */
static void	handle_child_process(t_cmd *cmd, t_minishell *minishell,
									struct sigaction *sa)
{
	int	err;

	if (sigaction(SIGUSR1, sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
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
	err = execute_path(cmd);
	destroy_cmd(cmd);
	free_minishell(minishell);
	exit(err);
}

/**
 * @brief Handle the parent process after fork
 *
 * @param t_cmd *cmd Command structure
 * @param t_minishell *minishell Minishell context
 */
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
	free_minishell(minishell);
	exit(err);
}

/**
 * @brief Execute a command in a child process
 * 			and manage input/output redirection.
 *
 * @param t_minishell *minishell Minishell context
 * @param t_cmd *cmd Command structure
 * @return int Exit code
 */
int	execute_external(t_minishell *minishell, t_cmd *cmd)
{
	struct sigaction	sa;
	pid_t				pid;

	setup_signals(&sa);
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("Error: fork failed\n", STDERR_FILENO);
		destroy_cmd(cmd);
		return (1);
	}
	else if (pid == 0)
	{
		if (is_builtin_command(cmd))
			handle_builtins_child_process(cmd, minishell);
		else
			handle_child_process(cmd, minishell, &sa);
	}
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	return (pid);
}
