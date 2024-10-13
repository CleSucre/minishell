/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 18:51:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/05 18:51:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Action when ctrl + c is pressed
 *          - reset input
 *          - print prompt
 *          - get cursor position
 *
 * @param t_minishell *minishell
 */
static void	ctrl_c_action(t_minishell *minishell)
{
	ft_fprintf(STDOUT_FILENO, "^C");
	reset_input(&minishell->input);
	print_terminal_prompt(minishell, 1);
	minishell->history_pos = 0;
	minishell->completion->tab_count = 0;
	minishell->completion->check_len = 0;
	minishell->completion->print_line = 1;
	get_cursor_position(minishell->term);
	minishell->term->begin_rows = minishell->term->rows;
	minishell->term->cols = get_prompt_len(minishell) + 1;
}

/**
 * @brief Process signals
 *
 * @param minishell
 * @param c
 * @param input
 * @return int 0 if not found, 1 if found and 2 if found and exit
 */
int	process_signals(t_minishell *minishell, char c)
{
	if (c == CTRL_D)
	{
		if (ft_tablen((const char **)minishell->input) == 0)
			return (2);
		return (1);
	}
	else if (c == CTRL_C)
	{
		ctrl_c_action(minishell);
		return (1);
	}
	return (0);
}
