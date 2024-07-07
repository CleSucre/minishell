/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 13:12:28 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/07 13:12:28 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_dict	*create_node(char *key, char *value)
{
	t_dict	*node;

	node = ft_calloc(1, sizeof(t_dict));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->left_branch = NULL;
	node->right_branch = NULL;
	node->parent = NULL;
	return (node);
}

t_dict	*insert_node(t_dict *root, char *key, char *value)
{
	t_dict	*node;

	node = create_node(key, value);
	if (!node)
		return (NULL);
	if (!root)
		return (node);
	if(ft_strncmp(key, root->key, ft_strlen(key)) < 0)
	{
		if (!root->left_branch)
		{
			root->left_branch = node;
			node->parent = root;
		}
		else
			insert_node(root->left_branch, key, value);
	}
	else
	{
		if (!root->right_branch)
		{
			root->right_branch = node;
			node->parent = root;
		}
		else
			insert_node(root->right_branch, key, value);
	}

}