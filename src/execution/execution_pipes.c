/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 23:51:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/05 23:51:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (ast->right->type == AST_REDIR_IN || ast->right->type == AST_HEREDOC)
	{
		close_fds(in_out, pipes);
		in_out[0] = STDIN_FILENO;
		in_out[1] = STDOUT_FILENO;
		in_out[2] = -1;
	}
	res = execute_ast(minishell, ast->right, pipes, in_out);
	if (res == 1)
		return (res);
	return (0);
}
