/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_cut_bst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/09/10 22:49:56 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Copy a specific branch in the BST
 *
 * @param t_dict *root Dict structure
 * @return t_dict* The new branch
 */
t_dict	*bst_copy(t_dict *root)
{
	t_dict	*new;

	if (!root)
		return (NULL);
	new = create_node(root->key, root->value);
	if (!new)
		return (NULL);
	if (root->left_branch)
		new->left_branch = bst_copy(root->left_branch);
	if (new->left_branch)
		new->left_branch->parent = new;
	if (root->right_branch)
		new->right_branch = bst_copy(root->right_branch);
	if (new->right_branch)
		new->right_branch->parent = new;
	return (new);
}

/**
 * @brief Cut a specific branch in the BST
 *
 * @param t_dict *root Dict structure
 * @param char *key Name specifically search in the BST
 */
void	cut_node(t_dict *root, char *key)
{
	if (!root)
		return ;
	if (ft_strncmp(key, root->key, ft_strlen(key)) != 0)
	{
		free_branch(root);
		root = NULL;
		return ;
	}
	if (root->right_branch && ft_strncmp
		(key, root->right_branch->key, ft_strlen(key)) != 0)
	{
		free_branch(root->right_branch);
		root->right_branch = NULL;
	}
	else if (root->right_branch)
		cut_node(root->right_branch, key);
	if (root->left_branch && ft_strncmp
		(key, root->left_branch->key, ft_strlen(key)) != 0)
	{
		free_branch(root->left_branch);
		root->left_branch = NULL;
	}
	else if (root->left_branch)
		cut_node(root->left_branch, key);
	return ;
}
