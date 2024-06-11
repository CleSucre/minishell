/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_alloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 03:58:37 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/05 03:58:37 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Init minishell structure
 *
 * @return t_minishell *
 */
t_minishell	*alloc_minishell(void)
{
	t_minishell	*minishell;

	minishell = ft_calloc(sizeof(t_minishell), 1);
	if (!minishell)
		return (NULL);
	minishell->cache = ft_calloc(sizeof(t_cache), 1);
	if (!minishell->cache)
	{
		free(minishell);
		return (NULL);
	}
	minishell->cache->input = ft_strdup("");
	minishell->history = ft_calloc(sizeof(t_history), 1);
	if (!minishell->history)
	{
		free(minishell->cache);
		free(minishell);
		return (NULL);
	}
	minishell->term = ft_calloc(sizeof(t_term), 1);
	if (!minishell->term)
	{
		free(minishell->history);
		free(minishell->cache);
		free(minishell);
		return (NULL);
	}
	get_terminal_size(minishell->term);
//	minishell->term->size_cols = 0;
	minishell->term->cols = ft_strlen(TERMINAL_PROMPT) + 1;
	minishell->term->rows = 0;

	minishell->exit_code = 0;
	minishell->history->cmd = NULL;
	minishell->history->older = NULL;
	minishell->history->newer = NULL;
	history_load(minishell);
	return (minishell);
}
