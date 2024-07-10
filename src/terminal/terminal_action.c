/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_action.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 05:39:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/06/03 05:39:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Erase a char in string
 *
 * @param minishell
 * @param input
 */
static void	backspace_action(t_minishell *minishell, char **input)
{
	if (minishell->term->cols != minishell->term->ws_cols
		&& minishell->term->cols % minishell->term->ws_cols == 1
		&& minishell->term->begin_rows > 0)
	{
		ft_putstr_fd("\033[A", 1);
		move_cursor_back(minishell->term->cols);
		move_cursor_forward(minishell->term->ws_cols);
		ft_trunc(input, 1);
		erase_term(0);
		minishell->term->cols--;
		ft_putstr_fd("\033[C", 1);
	}
	else if (ft_strlen(*input) > 0 && minishell->term->cols
		!= get_prompt_len(minishell) + ft_strlen(*input) + 1)
	{
		*input = erase_in_string(minishell, *input);
		minishell->term->cols--;
	}
	else if (ft_strlen(*input) > 0 && minishell->term->cols > 1)
	{
		ft_trunc(input, 1);
		erase_term(1);
		minishell->term->cols--;
	}
}

void	ctrl_c_action(t_minishell *minishell, char **input)
{
	terminal_print("^C", 0, STDOUT_FILENO);
	reset_input(input);
	print_terminal_prompt(minishell, 1);
	minishell->history_pos = 0;
	get_cursor_position(minishell->term);
}

void	edit_input(t_minishell *minishell, char **input, char c)
{
	if (minishell->term->cols
		!= get_prompt_len(minishell) + ft_strlen(*input) + 1)
		*input = put_in_string(minishell, *input, c);
	else
	{
		*input = ft_charjoin(*input, c);
		ft_putchar_fd(c, 1);
	}
	minishell->term->cols++;
}

/**
 * @brief Sort inputs && act in consequence
 *
 * @param t_minishell *minishell 	struct which access history
 * @param char c					char read by use_termios
 * @param char **input				string which join every char
 * 								read by termios from 1st to Enter
 * @return int 						1 if exit, 0 if not
 */

int	process_action(t_minishell *minishell, char c, char **input)
{
	if (c == CTRL_D && ft_strlen(*input) == 0)
		return (1);
	else if (c == CTRL_D)
		return (0);
	else if (c == CTRL_C)
		ctrl_c_action(minishell, input);
	else if (c == BACKSPACE)
		backspace_action(minishell, input);
	else if (c == CARRIAGE_RETURN || c == NEW_LINE)
	{
		terminal_print("", ft_strlen(*input) > 0, STDOUT_FILENO);
		if (execute(minishell, *input))
			return (1);
		print_terminal_prompt(minishell, ft_strlen(*input) <= 0);
		reset_input(input);
		minishell->history_pos = 0;
		get_cursor_position(minishell->term);
	}
	else if (c == ESC_SEQ)
	{
		if (interpret_escape_sequence(minishell, input, minishell->term->cols))
			return (0);
	}
	else
		edit_input(minishell, input, c);
	return (0);
}
