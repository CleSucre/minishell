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

/**
 *  @brief Duplicate a string with a specific length
 *  @return char * The duplicate string
 */

char	*ft_strndup(char *s1, int len)
{
	char	*res;
	int		i;

	res = (char *)ft_calloc(ft_strlen(s1) + 1, sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (s1[i] && i < len)
	{
		res[i] = s1[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

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
	node->key = key;
	node->value = value;
	node->left_branch = NULL;
	node->right_branch = NULL;
	node->parent = NULL;
	return (node);
}

/**
 * @brief Insert a new node in the BST according to an alphabetical order
 * @param root - parent node
 * @param key - name of directory
 * @param value - file/exec/..
 * @return
 */

t_dict	*insert_node(t_dict *root, char *key, char *value)
{
	t_dict	*node;

	key = ft_strndup(key, ft_strlen(key) + 1);
	node = create_node(key, value);
	if (!node)
		return (NULL);
	if (!root)
		return (node);
//	ft_putstr_fd("vla key: ", 1);
//	ft_putstr_fd(key, 1);
//	ft_putstr_fd("\n", 1);
	if(ft_strncmp(key, root->key, ft_strlen(key)) < 0)
	{
		if (!root->left_branch)
		{
			node->parent = root;
			root->left_branch = node;
		}
		else
			insert_node(root->left_branch, key, value);
//		ft_putstr_fd("left\n", 1);

	}
	else
	{
		if (!root->right_branch)
		{
			node->parent = root;
			root->right_branch = node;
		}
		else
			insert_node(root->right_branch, key, value);
//		ft_putstr_fd("right\n", 1);
	}
//	ft_putstr_fd("root key: ", 1);
//	ft_putstr_fd(node->key, 1);
//	ft_putstr_fd("\n", 1);
	return(root);
}

/**
 * @brief Search for a specific node in the BST
 * @param root Dict structure
 * @param key Name specifically search in the BST
 * @return
 */

t_dict	*search_node(t_dict *root, char *key)
{
	ft_putstr_fd("key: ", 1);
	ft_putstr_fd(key, 1);
	ft_putstr_fd("\n", 1);

	if (!root)
		return (NULL);
	ft_putstr_fd("root key: ", 1);
	ft_putstr_fd(root->key, 1);
	ft_putstr_fd("\n", 1);
	if (ft_strncmp(key, root->key, ft_strlen(key)) == 0)
		return (root);
	if (ft_strncmp(key, root->key, ft_strlen(key)) < 0)
		search_node(root->left_branch, key);
	else
		search_node(root->right_branch, key);
	return (NULL);
}

void	print_branch(t_dict *root)
{
	if (!root)
		return ;
	ft_putstr_fd(root->key, 1);
	ft_putstr_fd("\n", 1);
	print_branch(root->left_branch);
	print_branch(root->right_branch);
	return ;

}