/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_in_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:24:04 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/13 10:26:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Clear term from cursor to end
 *	Function i did before realised that \033[J is enough
 */
static void	clear_term(t_minishell *minishell)
{
	unsigned int	rows;

	rows = minishell->term->rows;
	while (rows > minishell->term->begin_rows)
	{
		ft_putstr_fd("\033[K\033[1A", 1);
		rows--;
	}
	ft_putstr_fd("\033[K", 1);
}

/**
 * 	@brief add new in minishell input then replace cursor at the right position
 */

static void	replace_cursor(t_minishell *minishell)
{
	unsigned int	tablen;
	unsigned int	prompt_len;

	prompt_len = get_prompt_len(minishell);
	tablen = ft_tablen((const char **)minishell->input);
	while (tablen + prompt_len > minishell->term->ws_cols + 2)
	{
		ft_putstr_fd("\033[A\r", 1);
		tablen = tablen - minishell->term->ws_cols;
	}
	ft_putstr_fd("\033[u\033[1C", 1);
}

static void	pis_other_rows(t_minishell *minishell, char *new)
{
	unsigned int	current_cols;
	unsigned int	current_rows;
	unsigned int	prompt_len;

	current_cols = minishell->term->cols;
	current_rows = minishell->term->rows;
	prompt_len = get_prompt_len(minishell);
	minishell->input = ft_tabinsert(minishell->input, new, ((current_rows
					- minishell->term->begin_rows) * minishell->term->ws_cols)
			- prompt_len + current_cols - 2);
	minishell->term->rows -= current_rows - minishell->term->begin_rows;
	clear_term(minishell);
	minishell->term->rows += current_rows - minishell->term->begin_rows;
	ft_putstr_fd(ft_utf8_tab_to_str(minishell->input) + ((current_rows
				- minishell->term->begin_rows) * minishell->term->ws_cols - 1)
		- prompt_len + current_cols - 1, STDOUT_FILENO);
}

void	put_in_string(t_minishell *minishell, char *new)
{
	unsigned int	current_rows;
	unsigned int	prompt_len;

	current_rows = minishell->term->rows;
	prompt_len = get_prompt_len(minishell);
	ft_putstr_fd("\033[s", 1);
	if (current_rows - minishell->term->begin_rows == 0)
	{
		minishell->input = ft_tabinsert(minishell->input, new,
				minishell->term->cols - prompt_len - 1);
		clear_term(minishell);
		ft_putstr_fd(ft_utf8_tab_to_str(minishell->input)
			+ (minishell->term->cols - prompt_len - 1), STDOUT_FILENO);
	}
	else
		pis_other_rows(minishell, new);
	replace_cursor(minishell);
}
