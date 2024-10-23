/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_or_and.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/17 11:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute || logic combined with && logic
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, 1 error
 */
static int	execute_or_with_and(t_minishell *minishell,
			t_ast_node *ast, int *pipes, int *in_out)
{
	int	res;

	if (minishell->exit_code == 0)
	{
		execute_ast(minishell, ast->right->left, pipes, in_out);
		if (minishell->exit_code == 130)
			res = 0;
		else
			res = minishell->exit_code;
		if (res == 0)
			return (0);
		execute_ast(minishell, ast->right->right, pipes, in_out);
	}
	else
	{
		res = execute_ast(minishell, ast->right->right, pipes, in_out);
		if (res != 0)
			return (0);
	}
	return (0);
}

/**
 * @brief Execute the ast respecting AND operator logic
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, 1 on exit request
 */
int	execute_and(t_minishell *minishell, t_ast_node *ast,
			int *pipes, int *in_out)
{
	int	res;

	execute_ast(minishell, ast->left, pipes, in_out);
	res = minishell->exit_code;
	if (res != 0)
		return (0);
	if (ast->right->type == AST_OR)
		return (execute_or_with_and(minishell, ast, pipes, in_out));
	else
	{
		res = execute_ast(minishell, ast->right, pipes, in_out);
		if (res != 0)
			return (0);
	}
	return (0);
}

/**
 * @brief Execute the ast respecting OR operator logic
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, 1 on exit request
 */
int	execute_or(t_minishell *minishell, t_ast_node *ast,
			int *pipes, int *in_out)
{
	int	res;

	res = execute_ast(minishell, ast->left, pipes, in_out);
	if (res != 0)
		return (0);
	if (minishell->exit_code == 0)
		return (minishell->exit_code);
	res = execute_ast(minishell, ast->right, pipes, in_out);
	if (res != 0)
		return (res);
	return (0);
}
