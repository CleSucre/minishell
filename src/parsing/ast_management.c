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
 * @brief Créer un nouveau nœud AST
 * @param type Le type du nœud
 * @param value La commande ou les arguments (peut être NULL pour certains nœuds)
 * @return t_ast_node* Le nœud AST créé
 */
t_ast_node	*new_ast_node(t_ast_node_type type, char **value)
{
	t_ast_node *node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = value;
	node->is_last = 0;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

/**
 * @brief Check if the AST contains a command from the given node
 *
 * @param ast The AST node to check
 * @return int 1 if the AST contains a command, 0 otherwise
 */
int contain_command(t_ast_node *ast)
{
	if (ast->type == AST_COMMAND)
		return (1);
	if (ast->left && contain_command(ast->left))
		return (1);
	if (ast->right && contain_command(ast->right))
		return (1);
	return (0);
}