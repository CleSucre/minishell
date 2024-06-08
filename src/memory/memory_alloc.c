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
 * @brief Alloc cache structure
 *
 * @param t_minishell *minishell
 * @return int 0 on success, 1 on failure
 */
static int	alloc_cache(t_minishell *minishell)
{
	minishell->cache = ft_calloc(sizeof(t_cache), 1);
	if (!minishell->cache)
	{
		free(minishell);
		return (1);
	}
	minishell->cache->input = ft_strdup("");
	minishell->cache->prompt
		= ft_strdup(TERMINAL_PROMPT_COLOR TERMINAL_PROMPT RESET);
	minishell->cache->prompt_len = ft_strlen(TERMINAL_PROMPT);
	return (0);
}

/**
 * @brief Alloc history structure
 *
 * @param t_minishell *minishell
 * @return int 0 on success, 1 on failure
 */
static int	alloc_history(t_minishell *minishell)
{
	minishell->history = ft_calloc(sizeof(t_history), 1);
	if (!minishell->history)
	{
		free(minishell->cache);
		free(minishell);
		return (1);
	}
	minishell->history->cmd = NULL;
	minishell->history->older = NULL;
	minishell->history->newer = NULL;
	history_load(minishell);
	return (0);
}

/**
 * @brief Alloc term structure
 *
 * @param t_minishell *minishell
 * @return int 0 on success, 1 on failure
 */
static int	alloc_term(t_minishell *minishell)
{
	minishell->term = ft_calloc(sizeof(t_term), 1);
	if (!minishell->term)
	{
		free(minishell->history);
		free(minishell->cache);
		free(minishell);
		return (1);
	}
	minishell->term->size = 0;
	minishell->term->cols = 0;
	minishell->term->rows = 0;
	return (0);
}

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
	minishell->exit_code = 0;
	if (alloc_cache(minishell))
		return (NULL);
	if (alloc_history(minishell))
		return (NULL);
	if (alloc_term(minishell))
		return (NULL);
	return (minishell);
}
