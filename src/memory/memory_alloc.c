/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_alloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 03:58:37 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/17 02:59:16 by julthoma         ###   ########.fr       */
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
	minishell->cache = ft_calloc(1, sizeof(t_cache));
	if (!minishell->cache)
	{
		free(minishell);
		return (1);
	}
	minishell->cache->input = ft_calloc(1, sizeof(char *));
	if (!minishell->cache->input)
	{
		free(minishell->cache);
		free(minishell);
		return (1);
	}
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
	minishell->history = ft_calloc(1, sizeof(t_history));
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
	minishell->term = ft_calloc(1, sizeof(t_term));
	if (!minishell->term)
	{
		free(minishell->history);
		free(minishell->cache);
		free(minishell);
		return (1);
	}
	get_terminal_size(minishell->term);
	minishell->term->cols = 0;
	minishell->term->rows = 0;
	minishell->term->begin_rows = 0;
	return (0);
}

static int	alloc_tab(t_minishell *minishell)
{
	minishell->completion = ft_calloc(1, sizeof(t_completion));
	if (!minishell->completion)
	{
		free(minishell->term);
		free(minishell->history);
		free(minishell->cache);
		free(minishell);
		return (1);
	}
	minishell->completion->tab_count = 0;
	minishell->completion->print_line = 1;
	minishell->completion->check_len = 0;
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

	minishell = ft_calloc(1, sizeof(t_minishell));
	if (!minishell)
		return (NULL);
	minishell->exit_code = 0;
	minishell->current_cmd = NULL;
	if (alloc_cache(minishell))
		return (NULL);
	if (alloc_history(minishell))
		return (NULL);
	if (alloc_term(minishell))
		return (NULL);
	if (alloc_tab(minishell))
		return (NULL);
	if (alloc_dirinfo(minishell))
		return (NULL);
//	if (alloc_currentcmd(minishell))	TODO: Make sure if i need to alloc it or not (Maxime)
//		return (NULL);
	minishell->input = ft_calloc(1, sizeof(char *));
	if (minishell->input == NULL)
		return (NULL);
	return (minishell);
}
