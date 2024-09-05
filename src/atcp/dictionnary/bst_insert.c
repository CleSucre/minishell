/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst_insert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <pierrot.maxime.pro@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 08:47:30 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/10 09:09:18 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Insert a new node in the BST as the left child of the root
 * @param root - parent node
 * @param key - name of directory
 * @param value - file/exec/..
 * @return
 */
static void	insert_l_node(t_dict *root, t_dict *node, char *key, char *value)
{
	if (!root->left_branch)
	{
		node->parent = root;
		root->left_branch = node;
	}
	else if (ft_strncmp(key, root->left_branch->key, ft_strlen(key)) < 0)
	{
		node->parent = root;
		node->left_branch = root->left_branch;
		root->left_branch = node;
	}
	else
		insert_node(root->left_branch, node, key, value);
}

/**
 * @brief Insert a new node in the BST as the right child of the root
 * @param root - parent node
 * @param key - name of directory
 * @param value - file/exec/..
 * @return
 */
static void	insert_r_node(t_dict *root, t_dict *node, char *key, char *value)
{
	if (!root->right_branch)
	{
		node->parent = root;
		root->right_branch = node;
	}
	else if (ft_strncmp(key, root->right_branch->key, ft_strlen(key)) < 0)
	{
		node->parent = root;
		node->right_branch = root->right_branch;
		root->right_branch = node;
	}
	else
		insert_node(root->right_branch, node, key, value);
}

/**
 * @brief Insert a new node in the BST according to an alphabetical order
 * @param root - parent node
 * @param key - name of directory
 * @param value - file/exec/..
 * @return
 */
t_dict	*insert_node(t_dict *root, t_dict *node, char *key, char *value)
{
	if (!node)
		return (NULL);
	if (!root)
		return (node);
	root->bst_size++;
	if (ft_strncmp(key, root->key, ft_strlen(key)) < 0)
		insert_l_node(root, node, key, value);
	else
		insert_r_node(root, node, key, value);
	return (root);
}
