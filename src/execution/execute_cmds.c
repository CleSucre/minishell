/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:13:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/09 18:13:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handle the pipe setup for the current command
 *
 * @param t_ast *ast
 * @param int fd[]
 * @param int in_out[]
 * @param int *to_close
 * @return int 0 on success, -1 on failure
 */
static int	setup_pipes(t_ast_node *ast, int *fd, int in_out[2], int *to_close)
{
	if (ast->next && ast->next->type == TOKEN_WORD)
	{
		if (pipe(fd) == -1)
		{
			ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
			return (-1);
		}
		in_out[1] = fd[1];
		*to_close = fd[0];
	}
	else
	{
		in_out[1] = STDOUT_FILENO;
		*to_close = -1;
	}
	return (0);
}

/**
 * @brief Close unnecessary file descriptors after command execution
 *
 * @param int in_out[]
 * @param int fd[]
 */
static void	close_fds(int in_out[2], int *fd)
{
	if (in_out[0] != STDIN_FILENO)
		close(in_out[0]);
	if (in_out[1] != STDOUT_FILENO)
		close(in_out[1]);
	if (fd[1] != -1)
		close(fd[1]);
}

/**
 * @brief Execute a single command
 *
 * @param t_minishell *minishell
 * @param t_ast *ast
 * @param int in_out[]
 * @param int to_close
 * @return int 0 on success, 1 on failure
 */
static int	execute_single_cmd(t_minishell *minishell, t_ast_node *ast,
		int in_out[2], int to_close)
{
	int	result;

	result = execute_cmd(minishell, ast, in_out, to_close);
	if (result)
	{
		if (result == 2)
			return (-1);
		ft_printf("Error: command failed\n");
		return (0);
	}
	return (1);
}

/**
 * @brief Main loop to execute commands from the AST
 *
 * @param t_minishell *minishell
 * @param t_ast *ast
 * @param int in_out[]
 * @param int fd[]
 * @return int 0 on success, -1 on failure
 */
static int	execute_commands_loop(t_minishell *minishell, t_ast_node *ast,
		int in_out[2], int *fd)
{
	int	to_close;
	int	result;

	to_close = -1;
	while (ast)
	{
		if (ast->type == TOKEN_WORD)
		{
			if (setup_pipes(ast, fd, in_out, &to_close) == -1)
				return (0);
			result = execute_single_cmd(minishell, ast, in_out, to_close);
			if (result <= 0)
				return (result);
			close_fds(in_out, fd);
			in_out[0] = fd[0];
		}
		ast = ast->next;
	}
	return (1);
}

/**
 * @brief Execute the commands based on the AST
 *
 * @param t_minishell *minishell
 * @param t_ast *ast
 * @return int 0 on success
 */
int	execute_cmds(t_minishell *minishell, t_ast_node *ast)
{
	int	fd[2];
	int	in_out[2];
	int	status;
	int	result;

	if (!ast)
		return (0);
	disable_termios(minishell->term);
	in_out[0] = STDIN_FILENO;
	in_out[1] = STDOUT_FILENO;
	fd[0] = -1;
	fd[1] = -1;
	result = execute_commands_loop(minishell, ast, in_out, fd);
	if (result == -1)
	{
		enable_termios(minishell->term);
		return (-1);
	}
	status = 0;
	status = wait_for_processes(&status);
	enable_termios(minishell->term);
	return (status);
}
