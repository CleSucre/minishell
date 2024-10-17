/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 23:51:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/05 23:51:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Close unnecessary file descriptors after command execution
 *
 * @param int in_out[2]
 * @param int *fd
 */
void	close_fds(int in_out[2], int *fd)
{
	if (in_out[0] != STDIN_FILENO)
		close(in_out[0]);
	if (in_out[1] != STDOUT_FILENO)
		close(in_out[1]);
	if (fd[1] != -1)
		close(fd[1]);
	in_out[0] = fd[0];
}

/**
 * @brief Setup pipes according to the command
 * 			if its a redirection or a simple pipe for the next cmd
 *
 * @param int *pipes pipe[0] = read, pipe[1] = write
 * @param int *in_out in_out[0] = read, in_out[1] = write, in_out[2] = to_close
 * @param int is_last 1 if its the last command, 0 otherwise
 * @return int 0 on success, -1 on failure
 */
int	setup_pipes(int *pipes, int *in_out, int is_last)
{
	if (!is_last)
	{
		if (pipe(pipes) == -1)
		{
			ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
			return (-1);
		}
		in_out[1] = pipes[1];
		in_out[2] = pipes[0];
	}
	else
	{
		in_out[1] = STDOUT_FILENO;
		in_out[2] = -1;
	}
	return (0);
}

/**
 * @brief Execute the ast respecting pipe operator logic
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, 1 on exit request
 */
int	execute_pipe(t_minishell *minishell, t_ast_node *ast,
					int *pipes, int *in_out)
{
	int	res;

	ast->left->in_pipe = 1;
	ast->right->in_pipe = 1;
	res = execute_ast(minishell, ast->left, pipes, in_out);
	if (res == 1)
		return (res);
	res = execute_ast(minishell, ast->right, pipes, in_out);
	if (res == 1)
		return (res);
	return (0);
}
