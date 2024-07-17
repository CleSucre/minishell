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
void	stress_print(char *str);
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

/**
 * @brief Copy a specific branch in the BST
 * @param root Dict structure
 * @return t_dict * The new branch
 */
t_dict	*bst_copy(t_dict *root)
{
	t_dict	*new;

	if (!root)
		return (NULL);
	new = create_node(root->key, root->value);
	if (root->left_branch)
		new->left_branch = bst_copy(root->left_branch);
	if (root->right_branch)
		new->right_branch = bst_copy(root->right_branch);
	return (new);
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
	if (root->left_branch)
		size += bst_size(root->left_branch);
	if (root->right_branch)
		size += bst_size(root->right_branch);
	return (size + 1);
}

/**
 * @brief Print the branch found
 * @param root Dict structure
 */
void	print_branch(t_minishell *minishell)
{
	if (!minishell->tab_dict)
	{
		ft_putstr_fd("NULL\n", 1);
		return ;
	}
	ft_putstr_fd(" key:   ", 1);
	ft_putstr_fd(minishell->tab_dict->key, 1);
	ft_putstr_fd("\n", 1);
	if (minishell->tab_dict->left_branch)
	{
//		ft_putstr_fd("left: ", 1);
		print_branch(minishell->tab_dict->left_branch);
	}
	if (minishell->tab_dict->right_branch)
	{
//		ft_putstr_fd("right: ", 1);
		print_branch(minishell->tab_dict->right_branch);
	}
//	ft_putstr_fd("end\n", 1);
	return ;
}

void	stress_print(t_minishell *minishell)
{
	if (ft_strlen(str) >= term->col)
	{
		ft_putstr_fd(str, 1);
		ft_putstr_fd("    ", 1);
	}
	else
	{
		ft_putstr_fd("\n", 1);
		ft_putstr_fd(str, 1);
	}
}