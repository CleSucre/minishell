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
static int	fill_dict(t_dict *dict, char *input);

//en gros je get path, je check le exec, si oui j ajoute a l ast. FIN
//J ajoute le mot complet au dico

int	search_in_path(t_dict *dict, char *odir)
{
	struct dirent *dir;
	DIR *d;
	(void)odir;
//	d = opendir("/sbin");
	d = opendir(odir);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if(access(dir->d_name, X_OK) != 0)
			{
				ft_putstr_fd(dir->d_name, 1);
				ft_putstr_fd("\n", 1);
//				fill_dict(dict, dir->d_name);
				insert_node(dict, dir->d_name, 0);
			}
		}
		closedir(d);
	}
	return (0);
}

static int	fill_dict(t_dict *dict, char *input)
{
	(void)dict;
	(void)input;
	return (0);
}

int	creation_dict(t_minishell *minishell)
{
	t_dict	*tmp;

	minishell->dict = ft_calloc(1, sizeof(t_dict));
	if (!minishell->dict)
		return (1);
	tmp = minishell->dict;
	search_in_path(tmp, "/sbin");
//	search_in_path(tmp, "/bin");
//	search_in_path(tmp, "/usr/sbin");
//	search_in_path(tmp, "/usr/local/bin");
//	search_in_path(tmp, "/opt/bin");
//	search_in_path(tmp, "/etc");

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