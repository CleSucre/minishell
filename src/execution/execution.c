/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 09:32:47 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handle the signal
 *
 * @param int sig
 * @eturn void
 */
void	handle_signal(int sig)
{
	if (sig == SIGUSR1)
		ft_putstr_fd("Received SIGUSR1 signal\n", STDOUT_FILENO);
}

/**
 * @brief Execute the command given in input
 *
 * @param t_cmd *cmd
 * @return int 0 on success, 1 on failure
 */
static int execute_path(t_cmd *cmd) {
	if (execve(cmd->path, cmd->argv, cmd->env) == -1) {
		ft_fprintf(STDERR_FILENO, "minishell: command not found: %s\n", cmd->cmd_name);
		exit(127);
	}
	return 0;
}

static int	execute_cmd(t_minishell *minishell, t_ast *ast, int input, int output, int to_close)
{
	t_cmd				*cmd;
	struct sigaction	sa;
	pid_t				pid;

	if (!ast)
		return (0);
	cmd = load_command(minishell, ast->children);
	if (!cmd)
		return (0);
	// Handle builtins
	/*
	if (is_builtin(cmd))
	{
		execute_builtin(minishell, cmd);
		free_cmd(cmd);
		return 1;
	}
	 */
	sa.sa_handler = handle_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("sigaction");
		return 1;
	}
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("Error: fork failed\n", STDERR_FILENO);
		free_cmd(cmd);
		return (0);
	}
	else if (pid == 0)
	{
		if (sigaction(SIGUSR1, &sa, NULL) == -1)
		{
			perror("sigaction");
			exit(EXIT_FAILURE);
		}
		if (input != STDIN_FILENO)
		{
			dup2(input, STDIN_FILENO);
			close(input);
		}
		if (output != STDOUT_FILENO)
		{
			dup2(output, STDOUT_FILENO);
			close(output);
		}

		if (to_close != -1)
			close(to_close);

		execute_path(cmd);
		free_cmd(cmd);
		exit(0);
	}
	if (input != STDIN_FILENO)
		close(input);
	if (output != STDOUT_FILENO)
		close(output);
	free_cmd(cmd);
	return (1);
}

/**
 * @brief Execute the ast
 *
 * @param t_minishell *minishell
 * @param t_ast *ast
 * @return int 1 on success, 0 on failure
 */
static int execute_cmds(t_minishell *minishell, t_ast *ast)
{
	int	fd[2];
	int input;
	int output;
	int to_close;

	if (!ast)
		return (0);
	disable_termios(minishell->term);
	input = STDIN_FILENO;
	while (ast)
	{
		if (ast->type == FULL_COMMAND)
		{
			if (ast->next && ast->next->type == FULL_COMMAND)
			{
				if (pipe(fd) == -1)
				{
					ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
					return (0);
				}
				output = fd[1];
				to_close = fd[0];
			}
			else
			{
				output = STDOUT_FILENO;
				to_close = -1;
			}

			if (!execute_cmd(minishell, ast, input, output, to_close))
			{
				if (input != STDIN_FILENO)
					close(input);
				if (output != STDOUT_FILENO)
					close(output);
				return (0);
			}

			if (input != STDIN_FILENO)
				close(input);
			if (output != STDOUT_FILENO)
				close(output);

			input = fd[0];
			if (ast->prev && ast->prev->type == FULL_COMMAND)
				close(fd[1]);
		}
		ast = ast->next;
	}

	// Wait for all children
	while (wait(NULL) > 0);
	enable_termios(minishell->term);

	return (1);
}


/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return int 1 on success, 0 on failure
 */
int	execute(t_minishell *minishell, char *input)
{
	t_ast	*ast;
	int		res;
	char 	*trimmed;

	if (ft_strlen(input) == 0)
		return (0);
	trimmed = ft_strtrim(input, WHITESPACES);
	if (!trimmed)
		return (0);
	debug_execution(trimmed);
	if (ft_isprint(*trimmed))
		history_add(minishell, trimmed, 1);
	ast = parse_input(minishell, trimmed);
	if (!ast)
	{
		free(trimmed);
		return (0);
	}
	free(trimmed);
	res = execute_cmds(minishell, ast);
	free_ast(ast);
	return (res);
}
