/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 09:53:22 by julthoma         ###   ########.fr       */
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
 * @return int error code
 */
static int	execute_path(t_cmd *cmd)
{
	int	err;

	err = execve(cmd->path, cmd->argv, cmd->env);
	if (err == -1)
		ft_fprintf(STDERR_FILENO, "minishell: command not found: %s\n",
			cmd->cmd_name);
	return (err);
}

static int	execute_cmd(t_minishell *minishell, t_ast *ast,
			int input, int output, int to_close)
{
	t_cmd				*cmd;
	struct sigaction	sa;
	pid_t				pid;
	int					err;

	if (!ast)
		return (1);
	cmd = load_command(minishell, ast->children, input, output);
	if (!cmd)
		return (1);
	if (is_builtin_command(cmd))
	{
		err = execute_builtin_command(minishell, cmd);
		free_cmd(cmd);
		return (err);
	}
	sa.sa_handler = handle_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (1);
	}
	err = 0;
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("Error: fork failed\n", STDERR_FILENO);
		free_cmd(cmd);
		return (1);
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
		err = execute_path(cmd);
		free_cmd(cmd);
		free_minishell(minishell);
		free_ast(ast);
		exit(err);
	}
	if (input != STDIN_FILENO)
		close(input);
	if (output != STDOUT_FILENO)
		close(output);
	free_cmd(cmd);
	return (err);
}

/**
 * @brief Execute the ast
 *
 * @param t_minishell *minishell
 * @param t_ast *ast
 * @return int 0 on success
 */
static int	execute_cmds(t_minishell *minishell, t_ast *ast)
{
	int	fd[2];
	int	input;
	int	output;
	int	to_close;
	int	status;
	int	result;

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
					enable_termios(minishell->term);
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
			result = execute_cmd(minishell, ast, input, output, to_close);
			if (result)
			{
				if (input != STDIN_FILENO)
					close(input);
				if (output != STDOUT_FILENO)
					close(output);
				if (result == 2)
				{
					enable_termios(minishell->term);
					return (-1);
				}
				ft_printf("Error: command failed\n");
				enable_termios(minishell->term);
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
	while ((wait(&status)) > 0)
		;
	enable_termios(minishell->term);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (0);
}

/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return 0 on success -1 on exit request
 */
int	execute(t_minishell *minishell, char *input)
{
	t_ast	*ast;
	int		res;
	char	*trimmed;

	if (ft_strlen(input) == 0)
	{
		terminal_print("", 1, STDOUT_FILENO);
		free(input);
		return (1);
	}
	trimmed = ft_strtrim(input, WHITESPACES);
	free(input);
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
	minishell->exit_code = res;
	return (res);
}
