/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/10 09:14:14 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Create a new AST node
 *
 * @param t_ast_node_type type AST node type
 * @param char **value The value of the node
 * 				(e.g., command name, parameter, etc.)
 * @return t_ast_node* The new AST node
 */
t_ast_node	*new_ast_node(t_ast_node_type type, char **value)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = value;
	node->is_last = 1;
	node->in_pipe = 0;
	node->heredoc_fd = -1;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

/**
 * @brief Count the number of heredoc nodes in the ast
 *
 * @param t_ast_node *ast
 * @return int
 */
int	count_heredoc(t_ast_node *ast)
{
	int	count;

	count = 0;
	if (!ast)
		return (0);
	if (ast->type == AST_HEREDOC)
		count++;
	count += count_heredoc(ast->left);
	count += count_heredoc(ast->right);
	return (count);
}

/**
 * @brief Find the next AST_COMMAND node and update its value
 *
 * @param char *new_command
 * @param t_ast_node *ast
 */
int	update_next_command(char *new_command, t_ast_node *ast)
{
	if (ast->type == AST_COMMAND)
	{
		ft_tabfree(ast->value);
		ast->value = ft_tabnew(1);
		ast->value[0] = ft_strdup(new_command);
		return (1);
	}
	else
	{
		if (ast->left && update_next_command(new_command, ast->left))
			return (1);
		if (ast->right && update_next_command(new_command, ast->right))
			return (1);
	}
	return (0);
}
