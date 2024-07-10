/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_dico.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 07:35:56 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/07 12:42:51 by mpierrot         ###   ########.fr       */
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
	struct dirent *dir;
	char *name;
	t_dict *tmp;
	DIR *d;
	d = opendir(odir);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if(access(dir->d_name, X_OK) != 0)
			{
				tmp = dict;
				name = ft_strndup(dir->d_name, (int)dir->d_reclen);
				name= string_lower(name);
				insert_node(tmp, name, 0);
				free(name);
			}
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
	char *search;

	search = "a";

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



/* TEST
 * int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
int main(void)
{
	char *s1="tfstf";
	char *s2="test";
	printf("%d\n", ft_strncmp(s1, s2, ft_strlen(s2)));

}
*/