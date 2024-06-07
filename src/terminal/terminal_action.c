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
 * @brief Erase a char in a string
 *
 * @param size_t len
 * @return void
 */
static void	erase_term(size_t len)
{
	size_t		i;

	i = 0;
	while (i < len)
	{
		ft_putstr_fd("\033[1D", 1);
		i++;
	}
	ft_putchar_fd(' ', 1);
	ft_putstr_fd("\033[1D", 1);
}

/**
 * @brief Print in our terminal
 * 			if nl, move cursor one line down
 * @param char * String to print
 * @param int nl Move cursor down and print newline(s)
 * @return void
 */
void	terminal_print(char *str, int nl)
{
	if (nl)
		ft_printf("\033[100D", str);
	while (nl--)
		ft_putchar_fd('\n', 1);
	ft_printf("%s", str);
}

/**
 * @brief Reset input string
 *
 * @param char **input
 * @return void
 */
void	reset_input(char **input)
{
	if (*input)
	{
		free(*input);
		*input = NULL;
	}
	*input = ft_calloc(sizeof(char *), 1);
}

/**
 * @brief Move cursor from cols to n positions
 * @param position
 */
void	move_cursor_back(size_t position)
{
	size_t	i;

	i = 0;
	while (i++ < position)
		ft_putstr_fd("\033[1D", 1);
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
	if (c == 4 && ft_strlen(*input) == 0)
		return (1);
	else if (c == 4)
		return (0);
	else if (c == 3)
	{
		terminal_print("^C", 0);
		reset_input(input);
		terminal_print(TERMINAL_PROMPT, 1);
		minishell->history_pos = 0;
	}
	else if (c == 127)
	{
		if (ft_strlen(*input) > 0 && minishell->term->cols
			!= ft_strlen(TERMINAL_PROMPT) + ft_strlen(*input) + 1)
			*input = erase_in_string(*input, minishell->term->cols);
		else if (ft_strlen(*input) > 0)
		{
			ft_trunc(input, 1);
			erase_term(1);
		}
	}
	else if (c == '\r' || c == '\n')
	{
		if (exec_command(minishell, *input))
			return (1);
		reset_input(input);
		terminal_print(TERMINAL_PROMPT, 1);
		minishell->history_pos = 0;
	}
	else if (c == '\033') //[ESC]
	{
		if (interpret_escape_sequence(minishell, input, minishell->term->cols))
			return (0);
	}
	else
	{
		if (minishell->term->cols
			!= ft_strlen(TERMINAL_PROMPT) + ft_strlen(*input) + 1)
		{
			ft_putstr_fd("\nBAD\n", 1);
			*input = put_in_string(*input, c, minishell->term->cols);
		}
		else
		{
			*input = ft_charjoin(*input, c);
			ft_putchar_fd(c, 1);
		}
	}
	return (0);
}
