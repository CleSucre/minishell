/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_alloc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/09/12 00:13:53 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	alloc_dirinfo(t_minishell *minishell)
{
	minishell->dirinfo = ft_calloc(1, sizeof(t_dirinfo));
	if (!minishell->dirinfo)
	{
		free(minishell->term);
		free(minishell->history);
		free(minishell->cache);
		free(minishell->tab_dict);
		free(minishell);
		return (1);
	}
	return (0);
}

int	alloc_starting_path(t_minishell *minishell)
{
	char	buffer[BUFFER_SIZE];
	char	*name;

	name = getcwd(buffer, BUFFER_SIZE + 1);
	minishell->starting_path = ft_calloc(ft_strlen(name) + 1, sizeof(char *));
	if (!minishell->starting_path)
	{
		free(minishell->term);
		free(minishell->history);
		free(minishell->cache);
		free(minishell->tab_dict);
		free(minishell->dirinfo);
		free(minishell);
		return (-1);
	}
	ft_strlcpy(minishell->starting_path, name, ft_strlen(name)+1);
	return (0);
}
