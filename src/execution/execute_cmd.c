/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: votre_nom <votre_email@example.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:13:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/23 12:00:00 by votre_nom        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

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
	ft_fprintf(STDERR_FILENO, "err: %d\n", err);
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
static int	execute_external(t_minishell *minishell, t_cmd *cmd)
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

/**
 * @brief Executes a command represented by an abstract syntax tree node
 *
 * @param t_minishell *minishell The minishell context
 * @param t_ast_node *ast The abstract syntax tree node
 * 				representing the command
 * @param int in_out[3] Array holding file descriptors
 * 				for input/output redirection
 * @return int Exit code of the command execution
 */
int	execute_cmd(t_minishell *minishell, t_ast_node *ast,
			int pipes[2], int in_out[3])
{
	t_cmd	*cmd;
	int		res;
	int		pid;

	res = 0;
	if (!ast)
		return (1);
	if (setup_pipes(pipes, in_out, ast->is_last) == -1)
	{
		ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
		return (1);
	}
	cmd = create_cmd(ast, minishell, in_out);
	if (!cmd)
		return (1);
	if (is_builtin_command(cmd) && !ast->in_pipe)
		res = execute_builtin_command(minishell, cmd);
	else
	{
		pid = execute_external(minishell, cmd);
		if (ast->is_last)
			res = wait_for_pid(pid);
	}
	destroy_cmd(cmd);
	close_fds(in_out, pipes);
	minishell->exit_code = res;
	return (res);
}
