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
	if (new->right_branch)
		new->right_branch->parent = new;
	if (new->left_branch)
		new->left_branch->parent = new;
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
	if (root->parent)
		root->position = root->parent->position + 1;
	else
		root->position = 0;
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
void	print_branch(t_dict *dict)
{
	if (!dict)
	{
		ft_putstr_fd("NULL\n", 1);
		return ;
	}
	ft_putstr_fd(" key:   ", 1);
	ft_putstr_fd(dict->key, 1);
	ft_putstr_fd("\n", 1);
	if (dict->left_branch)
	{
//		ft_putstr_fd("left: ", 1);
		print_branch(dict->left_branch);
	}
	if (dict->right_branch)
	{
//		ft_putstr_fd("right: ", 1);
		print_branch(dict->right_branch);
	}
//	ft_putstr_fd("end\n", 1);

	return ;
}

/**
 * @brief Search in the path for a specific node
 * @param dict Dict structure
 * @param key position in the BST searched
 */
t_dict *move_in_bst(t_dict *dict, int key)
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


/**
 * @brief Print string key X times
 * @param key string to print
 * @param x how many times to print
 */
void	print_key_x(char *key, int x)
{
	int	i;

	if (x < 0 || !key)
		return ;
	i = 0;
	while (i < x)
	{
		ft_putstr_fd(key, 1);
		i++;
	}
}

/**
 * @brief Print the branch found with stress condition
 * @param dict
 */
void	stress_print(t_dict *dict)
{
	t_term	*term;

	term = ft_calloc(1, sizeof(t_term));
	get_cursor_position(term);
	get_terminal_size(term);
	if (((ft_strlen(dict->key) + term->cols)
			+ term->ws_cols / 5) < term->ws_cols)
	{
		ft_putstr_fd(dict->key, 1);
		print_key_x(" ", (20 - ft_strlen(dict->key)));
	}
	else
	{
		ft_putstr_fd("\033[E", 1);
		ft_putstr_fd(dict->key, 1);
		print_key_x(" ", (20 - ft_strlen(dict->key)));
	}
	if (dict->left_branch)
		stress_print(dict->left_branch);
	if (dict->right_branch)
		stress_print(dict->right_branch);
	free(term);
}
