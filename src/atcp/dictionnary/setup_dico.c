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
 * @brief Open dir with odir path and fill dict with all files in it which are executable
 * 		return a BST full with all executable files found and sort alphabetically
 * 	@param	dict - BST
 * 	@param	odir - path to open
 * 	@return 0 if success
 */
int	search_in_path(t_dict *dict, char *odir)
{
	struct dirent	*dir;
	char			*name;
	t_dict			*tmp;
	t_dict *node;
	DIR				*d;

	d = opendir(odir);
	if (d)
	{
		dir = readdir(d);
		while (dir != NULL)
		{
			if (access(dir->d_name, X_OK) != 0)
			{
				tmp = dict;
				name = ft_strndup(dir->d_name, (int)dir->d_reclen);
				name = string_lower(name);
				node = create_node(name, "command");
				insert_node(tmp, node, name, "command");
				free(name);
				name = NULL;
			}
			dir = readdir(d);
		}
		closedir(d);
	}
	else
	{
		perror("opendir");
		return (1);
	}
	return (0);
}

/**
 * @brief 	Create first BST node (root)
 *			Fill it with search in path
 *			Search for a specific node
 *			Print the branch found
 * 	@return 0 if success
 */

int	creation_dict(t_minishell *minishell)
{
	t_dict	*tmp;

	minishell->dict = create_node("\0", "file");
	if (!minishell->dict)
		return (1);
	minishell->dict->bst_size = 0;
	tmp = minishell->dict;
//	search_in_path(tmp, "/bin");
	search_in_path(tmp, "/usr/sbin");
//	search_in_path(tmp, "/usr/local/bin");
//	search_in_path(tmp, "/opt/bin");
//	search_in_path(tmp, "/etc");
	return (0);
}

///usr/sbin
///usr/local/bin
///opt/bin
///etc