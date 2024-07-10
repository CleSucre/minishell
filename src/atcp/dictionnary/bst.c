/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 13:12:28 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/10 08:53:59 by mpierrot         ###   ########.fr       */
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
	node->key = key;
	node->value = value;
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
		return (root);
	if (ft_strncmp(key, root->key, ft_strlen(key)) < 0)
		return (search_node(root->left_branch, key));
	else
		return (search_node(root->right_branch, key));
}

/**
 * @brief Cut a specific branch in the BST
 * @param root Dict structure
 */

t_dict	*cut_node(t_dict *root, char *key)
{
	if (!root)
		return (NULL);
	if (ft_strncmp(key, root->key, ft_strlen(key)) != 0)
	{
		root->left_branch = NULL;
		root->right_branch = NULL;
		root = NULL;
		return (root);
	}
	else if (root->right_branch && ft_strncmp
		(key, root->right_branch->key, ft_strlen(key)) != 0)
		root->right_branch = NULL;
	else if (root->right_branch)
		cut_node(root->right_branch, key);
	if (root->left_branch && ft_strncmp
		(key, root->left_branch->key, ft_strlen(key)) != 0)
		root->left_branch = NULL;
	else if (root->left_branch)
		cut_node(root->left_branch, key);
	return (root);
}

///**
// * @brief Free a specific node in the BST
// * @param node Dict structure
// */
//void	free_node(t_dict *node)
//{
//	if (!node)
//		return ;
//	free(node->key);
//	free(node->value);
//	free(node);
//}
//
///**
// * @brief Free the branch found
// * @param root Dict structure
// */
//void	free_branch(t_dict *root)
//{
//	if (!root)
//		return ;
//	if (root->left_branch)
//		free_branch(root->left_branch);
//	if (root->right_branch)
//		free_branch(root->right_branch);
//	free(root->key);
//	free(root->value);
//	free(root);
//}

/**
 * @brief Print the branch found
 * @param root Dict structure
 */
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
	if (root->left_branch)
	{
//		ft_putstr_fd("left: ", 1);
		print_branch(root->left_branch);
	}
	if (root->right_branch)
	{
//		ft_putstr_fd("right: ", 1);
		print_branch(root->right_branch);
	}
//	ft_putstr_fd("end\n", 1);
	return ;
}
