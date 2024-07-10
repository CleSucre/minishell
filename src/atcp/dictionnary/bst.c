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
 * @brief Convert a string to lowercase
 * @param *str The string to convert
 * @return char * The converted string
 */

char *string_lower(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		str[i] = ft_tolower(str[i]);
		i++;
	}
	return (str);
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
	if(ft_strncmp(key, root->key, ft_strlen(key)) < 0)
	{
		if (!root->left_branch)
		{
			node->parent = root;
			root->left_branch = node;
		}
		else if(ft_strncmp(key, root->left_branch->key, ft_strlen(key)) < 0)
		{
			node->parent = root;
			node->left_branch = root->left_branch;
			root->left_branch = node;
		}
		else
			insert_node(root->left_branch, key, value);

	}
	else
	{
		if (!root->right_branch)
		{
			node->parent = root;
			root->right_branch = node;
		}
		else if(ft_strncmp(key, root->right_branch->key, ft_strlen(key)) < 0)
		{
			node->parent = root;
			node->right_branch = root->right_branch;
			root->right_branch = node;
		}
		else
		insert_node(root->right_branch, key, value);
	}
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
	if (!root)
		return (NULL);
	if (ft_strncmp(key, root->key, ft_strlen(key)) == 0)
		return (root);
	if (ft_strncmp(key, root->key, ft_strlen(key)) < 0)
		return(search_node(root->left_branch, key));
	else
		return(search_node(root->right_branch, key));
}

/**
 * @brief Cut a specific branch in the BST
 * @param root Dict structure
 */

t_dict *cut_node(t_dict *root, char *key)
{
	if(!root)
		return (NULL);
	if(ft_strncmp(key, root->key, ft_strlen(key)) != 0)
	{
		root->left_branch = NULL;
		root->right_branch = NULL;
		root = NULL;
		return(root);
	}
	else if(root->right_branch && ft_strncmp(key, root->right_branch->key, ft_strlen(key)) != 0)
		root->right_branch = NULL;
	else if (root->right_branch)
		cut_node(root->right_branch, key);
	if(root->left_branch && ft_strncmp(key, root->left_branch->key, ft_strlen(key)) != 0)
		root->left_branch = NULL;
	else if (root->left_branch)
		cut_node(root->left_branch, key);
	return(root);
}

void	print_branch(t_dict *root)
{
	if (!root)
	{
		ft_putstr_fd("NULL\n", 1);
		return ;
	}
	ft_putstr_fd(" key:   ", 1);
	ft_putstr_fd(root->key, 1);
	ft_putstr_fd("\n", 1);
	if(root->left_branch)
	{
//		ft_putstr_fd("left: ", 1);
		print_branch(root->left_branch);
	}
	if(root->right_branch)
	{
//		ft_putstr_fd("right: ", 1);
		print_branch(root->right_branch);
	}
//	ft_putstr_fd("end\n", 1);
	return ;

}