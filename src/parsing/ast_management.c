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
	node->is_last = 1;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

/**
 * @brief Check if the node corresponds to type variable given
 *
 * @param t_ast_node		*node
 * @param t_ast_node_type	*type
 * @return int 1 if the next node is a command, 0 otherwise
 */
int	is_node(t_ast_node *node, t_ast_node_type type)
{
	if (!node)
		return (0);
	if (node->type == type)
		return (1);
	return (0);
}
