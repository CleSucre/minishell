/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:13:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/09 18:13:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Set up signal handling for SIGUSR1
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
 */
static void	handle_child_process(t_cmd *cmd, t_minishell *minishell, t_ast *ast
		, struct sigaction *sa)
{
	int	err;

	if (sigaction(SIGUSR1, sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	if (cmd->input != STDIN_FILENO)
	{
		dup2(cmd->input, STDIN_FILENO);
		close(cmd->input);
	}
	if (cmd->output != STDOUT_FILENO)
	{
		dup2(cmd->output, STDOUT_FILENO);
		close(cmd->output);
	}
	if (cmd->to_close != -1)
		close(cmd->to_close);
	err = execute_path(cmd);
	free_cmd(cmd);
	free_minishell(minishell);
	free_ast(ast);
	exit(err);
}

static int	execution(t_minishell *minishell, t_cmd *cmd, t_ast *ast)
{
	struct sigaction	sa;
	pid_t				pid;

	setup_signals(&sa);
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("Error: fork failed\n", STDERR_FILENO);
		free_cmd(cmd);
		return (1);
	}
	else if (pid == 0)
		handle_child_process(cmd, minishell, ast, &sa);
	if (cmd->input != STDIN_FILENO)
		close(cmd->input);
	if (cmd->output != STDOUT_FILENO)
		close(cmd->output);
	return (0);
}

static int	pre_execute(t_minishell *minishell, t_cmd *cmd,
		t_ast *ast)
{
	int	res;

	res = 0;
	if (is_builtin_command(cmd))
		res = execute_builtin_command(minishell, cmd);
	else if (cmd->path)
		res = execution(minishell, cmd, ast);
	free_cmd(cmd);
	return (res);
}

/**
 * @brief Main function for executing a command
 */
int	execute_cmd(t_minishell *minishell, t_ast *ast, int in_out[2], int to_close)
{
	t_cmd	*cmd;
	int		res;

	if (!ast)
		return (1);
	cmd = load_command(minishell, ast->children, in_out, to_close);
	if (!cmd)
		return (1);
	res = pre_execute(minishell, cmd, ast);
	ft_fprintf(STDERR_FILENO, "Result: %d\n", res);
	return (res);
}
