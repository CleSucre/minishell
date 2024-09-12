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