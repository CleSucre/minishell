/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_dico.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 07:35:56 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/10 09:08:30 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Add a new node in the BST
 *
 * @param struct dirent *dir - directory
 * @param t_dict *dict - BST
 */
void	send_to_bst(struct dirent *dir, t_dict *dict)
{
	char			*name;
	t_dict			*tmp;
	t_dict			*node;

	tmp = dict;
	name = ft_strndup(dir->d_name, (int)dir->d_reclen);
	name = string_lower(name);
	node = create_node(name, "command");
	insert_node(tmp, node, name, "command");
	free(name);
	name = NULL;
}

/**
 * @brief Open dir with odir path and fill dict with all files in it
 * 		which are executable return a BST full with all
 * 		executable files found and sort alphabetically
 *
 * 	@param t_dict *dict - BST
 * 	@param char *odir - path to open
 * 	@return int 0 if success
 */
int	search_in_path(t_dict *dict, char *odir)
{
	struct dirent	*dir;
	DIR				*d;

	d = opendir(odir);
	if (d)
	{
		dir = readdir(d);
		while (dir != NULL)
		{
			if (access(dir->d_name, X_OK) != 0)
				send_to_bst(dir, dict);
			dir = readdir(d);
		}
		closedir(d);
	}
	else
	{
		perror("warning: opendir bst");
		return (1);
	}
	return (0);
}

/**
 * @brief Create first BST node (root)
 *			Fill it with search in path
 *			Search for a specific node
 *			Print the branch found
 *
 * @param t_minishell *minishell
 * @return int 0 if success
 */
int	creation_dict(t_minishell *minishell)
{
	t_dict	*tmp;
	char	**paths;
	int		i;

	minishell->dict = create_node("\0", "file");
	if (!minishell->dict)
		return (1);
	minishell->dict->bst_size = 0;
	tmp = minishell->dict;
	paths = ft_split(get_var_value_const(minishell->env, "PATH"), ":");
	if (!paths)
		return (1);
	i = 0;
	while (paths[i])
	{
//		search_in_path(tmp, paths[i]);
		i++;
	}
	ft_tabfree(paths);
	return (0);
}
