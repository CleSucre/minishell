/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_action.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 05:39:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/19 07:09:49 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Erase a char in string
 *
 * @param minishell
 * @param input
 */
static void	backspace_action(t_minishell *minishell)
{
	size_t	len;

	len = ft_tablen((const char **)minishell->input);
	if (minishell->term->cols != minishell->term->ws_cols
		&& minishell->term->cols % minishell->term->ws_cols == 1
		&& minishell->term->begin_rows > 0)
	{
		ft_tabdel(minishell->input, len - 1);
		erase_term(0);
		minishell->term->cols--;
		move_cursor_forward(1);
	}
	else if (len > 0 && minishell->term->cols != get_prompt_len(minishell) + 1)
	{
		erase_in_string(minishell);
		minishell->term->cols--;
	}
}

static int	process_tab(t_minishell *minishell, char *new)
{
	char	*str;

	if (new[0] == '\t' || (minishell->completion->check_len == 1
			&& (minishell->completion->tab_count == 0
				&& (new[0] == 'y' || new[0] == 'n'))))
		tab_manager(minishell, new);
	else if (new[0] == '\t')
	{
		str = ft_utf8_tab_to_str(minishell->input);
		tab_completion(minishell, &str);
		free(str);
	}
	else
		return (0);
	return (1);
}

/**
 * @brief Sort inputs && act in consequence
 *
 * @param t_minishell *minishell 	struct which access history
 * @param char c					char read by use_termios
 * 								read by termios from 1st to Enter
 * @return int 						1 if exit, 0 if not
 */
int	process_action(t_minishell *minishell, char *new)
{
	if (process_tab(minishell, new))
		return (0);
	else if (new[0] == BACKSPACE)
		backspace_action(minishell);
	else if (new[0] == CARRIAGE_RETURN && minishell->completion->tab_count != 0)
		prompt_completion(minishell, minishell->input);
	else if (new[0] == CARRIAGE_RETURN || new[0] == NEW_LINE)
	{
		if (ft_tablen((const char **)minishell->input) > 0)
			ft_fprintf(STDOUT_FILENO, "\n");
		if (execute_input(minishell, ft_utf8_tab_to_str(minishell->input)) == -1)
			return (1);
		print_terminal_prompt(minishell, 0);
		reset_input(&minishell->input);
		minishell->history_pos = 0;
		get_cursor_position(minishell->term);
		minishell->term->cols = get_prompt_len(minishell) + 1;
		minishell->term->begin_rows = minishell->term->rows;
	}
	else if (new[0] == ESC_SEQ)
		return (!interpret_escape_sequence(minishell, new));
	else
		edit_input(minishell, new);
	return (0);
}
