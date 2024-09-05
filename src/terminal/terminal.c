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
 * @param t_minishell	Struct which access to history
 * @param input			Current input from user
 * @return int			1 if an action is done, 0 if not
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
 * @brief Set tabstop every 4, will possibly be deleted
 */
void	set_tabstop(t_minishell *minishell)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	ft_putstr_fd("\033[s", 1);
	ft_putstr_fd("\033[3g", 1);
	while (j < minishell->term->ws_rows)
	{
		while (i < minishell->term->ws_cols)
		{
			i += 4;
			ft_putstr_fd("\033[4C", 1);
			ft_putstr_fd("\033H", 1);
		}
		ft_putstr_fd("\033[B", 1);
		j++;
		i = 0;
	}
	ft_putstr_fd("\033[u", 1);
}

/**
 * @brief Use termios to get input from user and process it
 *
 * @param t_minishell *minishell
 * @return int 0 if no error, 1 if error
 */
int	use_termios(t_minishell *minishell)
{
	char	buffer[256];
	int		signal;
	ssize_t	bits;

	set_tabstop(minishell);
	creation_dict(minishell);
	print_terminal_prompt(minishell, 1);
	get_cursor_position(minishell->term);
	while (1)
	{
		get_terminal_size(minishell->term);
		minishell->term->begin_rows
			= ft_tablen((const char **)minishell->input) % MAX_32_BIT;
		bits = read(STDIN_FILENO, &buffer, sizeof(buffer));
		if (bits == -1)
		{
			perror("read");
			return (1);
		}
		buffer[bits] = '\0';
		signal = process_signals(minishell, buffer[0]);
		if (signal == 1)
		{
			ft_bzero(buffer, sizeof(buffer));
			continue ;
		}
		if (signal == 2 || process_action(minishell, buffer))
			break ;
		ft_bzero(buffer, sizeof(buffer));
	}
	ft_fprintf(STDOUT_FILENO, "\n");
	ft_fprintf(STDOUT_FILENO, TERMINAL_EXIT_MSG);
	ft_fprintf(STDOUT_FILENO, "\n");
	return (0);
}
