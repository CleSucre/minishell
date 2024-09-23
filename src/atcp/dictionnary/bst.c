/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 13:12:28 by mpierrot          #+#    #+#             */
/*   Updated: 2024/08/18 01:04:21 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * @brief Create a new node
 * @param *key name of directory
 * @param *value file/exec/..
 * @param left_branch // right_branch - Childrens
 * @param parent - Parent node
 * @return t_dict * The new node
 */

t_dict	*create_node(char *key, char *value)
{
	t_dict	*node;

	node = ft_calloc(1, sizeof(t_dict));
	if (!node)
		return (NULL);
	node->key = ft_strndup(key, ft_strlen(key));
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	node->value = ft_strndup(value, ft_strlen(value));
	if (!node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	node->position = 0;
	node->left_branch = NULL;
	node->right_branch = NULL;
	node->parent = NULL;
	return (node);
}

/**
 * @brief Search for a specific node in the BST
 * @param root Dict structure
 * @param key Name specifically search in the BST
 * @return
 */

t_dict	*search_node(t_dict *root, char *key)
{
	if (!root)
		return (NULL);
	if (ft_strncmp(key, root->key, ft_strlen(key)) == 0)
		return (bst_copy(root));
	if (ft_strncmp(key, root->key, ft_strlen(key)) < 0)
		return (search_node(root->left_branch, key));
	else
		return (search_node(root->right_branch, key));
}

/**
 * @brief Get the Number of Branch in the BSTn
 * @param root Dict structure
 * @return size_t The size of the BST
 */
size_t	bst_size(t_dict *root)
{
	size_t	size;

	size = 0;
	if (!root)
		return (0);
	if (root->parent)
		root->position = root->parent->position + 1;
	else
		root->position = 1;
	if (root->left_branch)
		size += bst_size(root->left_branch);
	if (root->right_branch)
		size += bst_size(root->right_branch);
	return (size + 1);
}

/**
 * @brief Move in the path for a specific node
 * @param dict Dict structure
 * @param key position in the BST searched
 */
t_dict	*move_in_bst(t_dict *dict, int key)
{
	if (!dict)
		return (NULL);
	while (dict->position != key)
	{
		if (dict->right_branch)
			return (move_in_bst(dict->right_branch, key));
		if (dict->left_branch)
			return (move_in_bst(dict->left_branch, key));
	}
	return (dict);
}
