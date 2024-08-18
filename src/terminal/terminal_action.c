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
static void	backspace_action(t_minishell *minishell, char **input)
{
	size_t len;

	len = ft_tablen((const char **)input);
	if (minishell->term->cols != minishell->term->ws_cols
		&& minishell->term->cols % minishell->term->ws_cols == 1
		&& minishell->term->begin_rows > 0)
	{
		ft_putstr_fd("\033[A", 1);
		move_cursor_back(minishell->term->cols);
		move_cursor_forward(minishell->term->ws_cols);
		ft_tabdel(input, len - 1);
		erase_term(0);
		minishell->term->cols--;
		ft_putstr_fd("\033[C", 1);
	}
	else if (len > 0 && minishell->term->cols != get_prompt_len(minishell) + 1)
	{
		erase_in_string(minishell, &input);
		minishell->term->cols--;
	}
	else if (len > 0 && minishell->term->cols != get_prompt_len(minishell) + 1)
	{
		ft_tabdel(input, len - 1);
		erase_term(1);
		minishell->term->cols--;
	}
}

void	ctrl_c_action(t_minishell *minishell, char ***input)
{
	terminal_print("^C", 0, STDOUT_FILENO);
	reset_input(input);
	print_terminal_prompt(minishell, 1);
	minishell->history_pos = 0;
	minishell->completion->tab_count = 0;
	minishell->completion->check_len = 0;
	minishell->completion->print_line = 1;
	get_cursor_position(minishell->term);
}

void	edit_input(t_minishell *minishell, char ***input, char *new)
{
	if (minishell->completion->check_len == 1)
	{
		minishell->completion->check_len = 0;
		ft_putstr_fd(**input, 1);
		return ;
	}
	if (minishell->term->cols
		!= get_prompt_len(minishell) + ft_tablen((const char **)*input) + 1)
        put_in_string(minishell, input, new);
	else
	{
		*input = ft_tabjoin(*input, ft_utf8_split_chars(new));
		ft_putstr_fd(new, STDOUT_FILENO);
	}
    minishell->term->cols = ft_tablen((const char **)*input) + get_prompt_len(minishell) + 1;
	minishell->completion->tab_count = 0;
	minishell->completion->check_len = 0;
	minishell->completion->print_line = 1;
}

/**
 * @brief Process signals
 *
 * @param minishell
 * @param c
 * @param input
 * @return int 0 if not found, 1 if found and 2 if found and exit
 */
int	process_signals(t_minishell *minishell, char c, char ***input)
{
	if (c == CTRL_D && ft_tablen((const char **)*input) == 0)
		return (2);
	else if (c == CTRL_D)
		return (1);
	else if (c == CTRL_C)
	{
		ctrl_c_action(minishell, input);
		return (1);
	}
	return (0);
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
int	process_action(t_minishell *minishell, char *new, char ***input)
{
    char    *str;
    char    c;

    c = new[0];
	if (c == CTRL_D && ft_strlen(**input) == 0)
		return (1);
	else if (c == CTRL_D)
		return (0);
	else if (c == '\t' || minishell->completion->check_len == 1
			&& (minishell->completion->tab_count == 0 && (c == 'y' || c == 'n')))
		tab_manager(minishell, *input, c);
	else if (c == CTRL_C)
		ctrl_c_action(minishell, input);
	else if (c == BACKSPACE)
		backspace_action(minishell, *input);
	else if (c == CARRIAGE_RETURN && minishell->completion->tab_count != 0)
		prompt_completion(minishell, *input);
	else if (c == CARRIAGE_RETURN || c == NEW_LINE)
	{
		terminal_print("", ft_tablen((const char **)*input) > 0, STDOUT_FILENO);
		str = ft_utf8_tab_to_str(*input);
		if (execute(minishell, str) == 0)
		{
			free(str);
			return (1);
		}
		free(str);
		set_tabstop(minishell);
		print_terminal_prompt(minishell, 0);
		reset_input(input);
		minishell->history_pos = 0;
		get_cursor_position(minishell->term);
	}
	else if (new[0] == ESC_SEQ)
	{
		if (interpret_escape_sequence(minishell, new, input))
			return (0);
	}
	else if (new[0] == '\t')
	{
		ft_putstr_fd("EHEIOHDFOEHBFEIOPHFIFHEIOPEFHOIEHF", 1);
//		str = ft_utf8_tab_to_str(*input);
//		tab_completion(minishell, &str);
		free(*input);
	}
	else
		edit_input(minishell, input, new);
	return (0);
}
