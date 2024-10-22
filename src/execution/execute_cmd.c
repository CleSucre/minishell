/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:13:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/23 12:00:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/**
 * @brief Decide whether to execute a command as a builtin
 * 			or external command and execute it
 *
 * @param t_cmd *cmd Command structure
 * @param t_minishell *minishell Minishell context
 * @return int 0 on success, 1 on exit request
 */
static int	decide_execution(t_cmd *cmd, t_minishell *minishell,
						t_ast_node *ast)
{
	int	res;
	int	pid;

	res = 0;
	if (is_builtin_command(cmd) && !ast->in_pipe)
		res = execute_builtin_command(minishell, cmd);
	else
	{
		pid = execute_external(minishell, cmd);
		if (ast->is_last)
		{
			wait_for_pid(pid);
			res = wait_for_processes();
		}
	}
	minishell->exit_code = res;
	return (res);
}

/**
 * @brief Executes a command represented by an abstract syntax tree node
 *
 * @param t_minishell *minishell The minishell context
 * @param t_ast_node *ast The abstract syntax tree node
 * 				representing the command
 * @param int in_out[3] Array holding file descriptors
 * 				for input/output redirection
 * @return int 0 on success, 1 on exit request
 */
int	execute_cmd(t_minishell *minishell, t_ast_node *ast,
			int pipes[2], int in_out[3])
{
	t_cmd	*cmd;

	if (setup_pipes(pipes, in_out, ast->is_last) == -1)
	{
		ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
		return (1);
	}
	cmd = create_cmd(ast, minishell, in_out);
	if (!cmd)
		return (0);
	if (decide_execution(cmd, minishell, ast) == 130)
	{
		minishell->exit_signal = cmd->exit_signal;
		destroy_cmd(cmd);
		close_fds(in_out, pipes);
		return (1);
	}
	minishell->exit_signal = cmd->exit_signal;
	destroy_cmd(cmd);
	close_fds(in_out, pipes);
	if (ast->is_last)
	{
		in_out[0] = STDIN_FILENO;
		in_out[1] = STDOUT_FILENO;
		in_out[2] = -1;
	}
	return (minishell->exit_signal);
}
