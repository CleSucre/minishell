/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <pierrot.maxime.pro@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 09:15:11 by mpierrot          #+#    #+#             */
/*   Updated: 2024/08/12 04:06:15 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Free a specific node in the BST
 *
 * @param t_dict *node Dict structure
 */
void	free_node(t_dict *node)
{
	if (!node)
		return ;
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
	node = NULL;
}

/**
 * @brief Free the branch found
 *
 * @param t_dict *root Dict structure
 */
void	free_branch(t_dict *root)
{
	if (!root)
		return ;
	if (root->left_branch)
		free_branch(root->left_branch);
	if (root->right_branch)
		free_branch(root->right_branch);
	free_node(root);
	root = NULL;
}
