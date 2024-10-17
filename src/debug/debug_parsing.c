/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/10 09:14:52 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Helper function to print child AST nodes
 *
 * @param child Child AST node
 * @param level Indentation level
 * @param label Label to print before the child node
 */
static void	debug_ast_child(t_ast_node *child, int level, const char *label)
{
	if (child)
	{
		ft_printf("%s\n", label);
		debug_children(child, level + 1);
	}
}

/**
 * @brief Print tokens list for debugging purposes
 *
 * @param tokens List of tokens
 */
void	debug_tokens(t_token *tokens)
{
	int		i;

	if (!DEBUG)
		return ;
	i = 0;
	while (tokens)
	{
		ft_printf("\n%s[DEBUG] ====== tokens [%d] ======%s\n", BLUE, i, RESET);
		ft_printf("token: %s%s%s\n", YELLOW, tokens->value, RESET);
		ft_printf("type str: %s%s%s (id: %d)\n",
			BOLDWHITE, get_token_type(tokens->type),
			RESET, tokens->type);
		tokens = tokens->next;
		i++;
	}
}

/**
 * @brief Print children of an AST node for debugging purposes
 *
 * @param ast AST node
 * @param level Level of the AST node (used for indentation)
 */
void	debug_children(t_ast_node *ast, int level)
{
	if (!DEBUG || !ast)
		return ;
	debug_ast_node(ast, level);
	debug_ast_child(ast->left, level, "Left next:");
	debug_ast_child(ast->right, level, "Right next:");
}

/**
 * @brief Print AST for debugging purposes
 *
 * @param ast AST node
 */
void	debug_ast(t_ast_node *ast)
{
	int	i;

	if (!DEBUG)
		return ;
	ft_printf("\n%s[DEBUG] ====== AST NODE ======%s\n", BLUE, RESET);
	ft_printf("type: %s%s%s (id: %d) | is_last: %d\n", BOLDWHITE,
		get_ast_node_type_primary(ast->type), RESET, ast->type, ast->is_last);
	i = 0;
	if (ast->type == AST_COMMAND)
	{
		ft_printf("command and args: ");
		while (ast->value && ast->value[i])
		{
			ft_printf("%s%s ", YELLOW, ast->value[i]);
			i++;
		}
		ft_printf("%s\n", RESET);
	}
	debug_ast_child(ast->left, 1, "Left next:");
	debug_ast_child(ast->right, 1, "Right next:");
}
