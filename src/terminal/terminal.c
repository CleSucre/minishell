/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 05:39:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/19 09:52:04 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Specifically for escape sequence as up-down-left-right arrow
 * 			- Left & right : Move cursor left and right as bash
 * 			- Up & Down : 	go search familiar
 * 				input user already wrote in .ministory
 *
 * @param t_minishell *minishell Minishell structure containing history data
 * @param const char *seq Escape sequence
 * @return int 1 if an action is done, 0 if not
 */
int	interpret_escape_sequence(t_minishell *minishell, const char *seq)
{
	t_history	*new_history;

	if (seq[1] == '[')
	{
		new_history = NULL;
		if (seq[2] == U_ARROW)
			arrow_up_action(minishell, new_history);
		else if (seq[2] == D_ARROW)
			arrow_down_action(minishell, new_history);
		else if (seq[2] == R_ARROW && minishell->term->cols
			< ft_tablen((const char **)minishell->input)
			+ get_prompt_len(minishell) + 1)
			arrow_right_action(minishell);
		else if (seq[2] == L_ARROW && minishell->term->cols
			> get_prompt_len(minishell) + 1)
			arrow_left_action(minishell);
		return (1);
	}
	return (0);
}

/**
 * @brief Initialize terminal and set tabstop before processing user input
 *
 * @param t_minishell *minishell
 */
static void	begin_user_input(t_minishell *minishell)
{
	set_tabstop(minishell);
	creation_dict(minishell);
	print_terminal_prompt(minishell, 1);
	get_cursor_position(minishell->term);
}

/**
 * @brief Process user input using termios
 *
 * @param t_minishell *minishell
 * @param int signal
 * @param ssize_t bits
 * @return int 0 if no error, 1 if error
 */
int	process_user_input(t_minishell *minishell,
				int signal, ssize_t bits)
{
	char	buffer[32];

	get_terminal_size(minishell->term);
	minishell->term->begin_rows
		= ft_tablen((const char **)minishell->input) % MAX_32_BIT;
	bits = read(STDIN_FILENO, &buffer, sizeof(buffer));
	if (bits == -1)
	{
		perror("read");
		return (0);
	}
	buffer[bits] = '\0';
	signal = process_signals(minishell, buffer[0]);
	if (signal == 1)
		return (1);
	if (signal == 2 || process_action(minishell, buffer))
		return (0);
	return (1);
}

/**
 * @brief Use termios to get input from user and process it
 *
 * @param t_minishell *minishell
 * @return void
 */
void	use_termios(t_minishell *minishell)
{
	int		signal;
	ssize_t	bits;

	signal = 0;
	bits = 0;
	begin_user_input(minishell);
	while (process_user_input(minishell, signal, bits))
		continue ;
	ft_fprintf(STDOUT_FILENO, "\n%s\n", TERMINAL_EXIT_MSG);
}
