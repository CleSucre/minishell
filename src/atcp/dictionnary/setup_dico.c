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
 * 	@return 0 if success
 */
int	search_in_path(t_dict *dict, char *odir)
{
	struct dirent	*dir;
	char			*name;
	t_dict			*tmp;
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
				insert_node(tmp, name, 0);
				free(name);
			}
			dir = readdir(d);
		}
		closedir(d);
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
	char	*search;

	search = "app";

	minishell->dict = create_node("\0", "file");
	if (!minishell->dict)
		return (1);
	tmp = minishell->dict;
//	search_in_path(tmp, ".");
	search_in_path(tmp, "/bin");
	search_in_path(tmp, "/usr/sbin");
	search_in_path(tmp, "/usr/local/bin");
	search_in_path(tmp, "/opt/bin");
	search_in_path(tmp, "/etc");

//	ft_putstr_fd("\nBefore\n", 1);
//	print_branch(tmp);


//	ft_putstr_fd("\nSearch\n", 1);
	tmp = search_node(minishell->dict, search);


//	ft_putstr_fd("CUT\n", 1);
	tmp = cut_node(tmp, search);

	ft_putstr_fd("\nFound\n", 1);
	print_branch(tmp);
	return (0);
}

///usr/sbin
///usr/local/bin
///opt/bin
///etc