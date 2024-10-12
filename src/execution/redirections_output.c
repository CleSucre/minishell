/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_output.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 00:14:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/13 00:14:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute the ast before redirecting the output to a file
 * 				and check if the file is accessible before doing so
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, -1 on failure
 */
static int	redirect_output(t_minishell *minishell, t_ast_node *ast,
						int *pipes, int *in_out)
{
	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no file specified\n", STDERR_FILENO);
		return (1);
	}
	if (access(ast->right->value[0], W_OK) != 0)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: %s: Permission denied\n", ast->right->value[0]);
		return (1);
	}
	execute_ast(minishell, ast->left, pipes, in_out);
	return (0);
}

/**
 * @brief Redirect the output of the command to a file
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, -1 on failure
 */
int	execute_redirect_output(t_minishell *minishell, t_ast_node *ast,
						int *pipes, int *in_out)
{
	int	file_fd;

	if (redirect_output(minishell, ast, pipes, in_out))
		return (1);
	file_fd = open(ast->right->value[0],
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
	{
		ft_putstr_fd("Error: open failed\n", STDERR_FILENO);
		return (1);
	}
	copy_fd_contents(in_out[0], file_fd);
	close(file_fd);
	close_fds(in_out, pipes);
	wait_for_processes();
	return (0);
}

/**
 * @brief Redirect the output of the command to a file
 * 			by appending the output to the file
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @param int append
 * @return int 0 on success, -1 on failure
 */
int	execute_redirect_output_append(t_minishell *minishell, t_ast_node *ast,
								int *pipes, int *in_out)
{
	int	file_fd;

	if (redirect_output(minishell, ast, pipes, in_out))
		return (1);
	file_fd = open(ast->right->value[0],
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (file_fd == -1)
	{
		ft_putstr_fd("Error: open failed\n", STDERR_FILENO);
		return (1);
	}
	copy_fd_contents(in_out[0], file_fd);
	close(file_fd);
	close_fds(in_out, pipes);
	wait_for_processes();
	return (0);
}