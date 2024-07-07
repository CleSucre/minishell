/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_dico.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 07:35:56 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/07 07:38:10 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//en gros je get path, je check le exec, si oui j ajoute a l ast. FIN
//J ajoute le mot complet au dico

int	search_in_path(t_dict *dict)
{
	struct dirent *dir;
	DIR *d;
	(void)dict;
	d = opendir("PATH");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			printf("%s\n", dir->d_name);
		}
		closedir(d);
	}
	return (0);
}

int	creation_dict(t_minishell *minishell)
{
	minishell->dict = ft_calloc(1, sizeof(t_dict));
	if (!minishell->dict)
		return (1);
	return (0);
}