/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 03:58:37 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/05 03:58:37 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Free history linked list
 *
 * @param t_history *history
 */
static void	history_free(t_history *history)
{
	t_history	*tmp;

	while (history)
	{
		tmp = history;
		history = history->older;
		free(tmp->cmd);
		free(tmp);
	}
}

/**
 * @brief Free minishell structure
 *
 * @param t_minishell *minishell
 */
void	free_minishell(t_minishell *minishell)
{
	free(minishell->cache->input);
	free(minishell->cache);
	//free(minishell->term->iterm_buffer);
	//free(minishell->term->strterm_buffer);
	free(minishell->term);
	history_free(minishell->history);
	free(minishell);
}
