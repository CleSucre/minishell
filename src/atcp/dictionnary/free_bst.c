/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <pierrot.maxime.pro@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 09:15:11 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/10 09:15:11 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Free a specific node in the BST
 * @param node Dict structure
 */
void	free_node(t_dict *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
	node = NULL;
}

/**
 * @brief Free the branch found
 * @param root Dict structure
 */
void	free_branch(t_dict *root)
{
	if (!root)
		return ;
	if (root->left_branch)
		free_branch(root->left_branch);
	if (root->right_branch)
		free_branch(root->right_branch);
	free(root->key);
	free(root->value);
	free(root);
	root = NULL;
}
