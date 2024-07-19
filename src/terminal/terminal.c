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
int	interpret_escape_sequence(t_minishell *minishell, const char *seq, char ***input)
{
	t_history	*new_history;

	if (seq[1] == '[')
	{
		new_history = NULL;
		if (seq[2] == U_ARROW)
			arrow_up_action(minishell, input, new_history);
		else if (seq[2] == D_ARROW)
			arrow_down_action(minishell, input, new_history);
		else if (seq[2] == R_ARROW && minishell->term->cols < ft_tablen((const char **)*input) + get_prompt_len(minishell) + 1)
			arrow_right_action(minishell);
		else if (seq[2] == L_ARROW && minishell->term->cols > get_prompt_len(minishell) + 1)
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

	i = 0;
	ft_putstr_fd("\033[3g", 1);
	while (i < minishell->term->cols)
	{
		i += 4;
		ft_putstr_fd("\033[4C", 1);
		ft_putstr_fd("\033H", 1);
	}
	ft_putstr_fd("\033[1G", 1);
}

/**
 * @brief Use termios to get input from user and process it
 *
 * @param t_minishell *minishell
 * @return int 0 if no error, 1 if error
 */
int	use_termios(t_minishell *minishell)
{
	char	**input;
	char	buffer[256];
	int		signal;
	ssize_t bits;

	input = ft_calloc(1, sizeof(char *));
	if (input == NULL)
		return (1);

	set_tabstop(minishell);
	print_terminal_prompt(minishell, 1);
	get_cursor_position(minishell->term);
	while (1)
	{
		get_terminal_size(minishell->term);
		minishell->term->begin_rows = ft_tablen((const char **)input) % 4294967295;
		bits = read(STDIN_FILENO, &buffer, sizeof(buffer));
		if (bits == -1)
		{
			perror("read");
			return (1);
		}
		buffer[bits] = '\0';
		signal = process_signals(minishell, buffer[0], &input);
		if (signal == 1)
			continue ;
		if (signal == 2)
			break ;
		if (process_action(minishell, buffer, &input))
			break ;
	}
	terminal_print(TERMINAL_EXIT_MSG, 1, STDOUT_FILENO);
	ft_printf("\nexit %d\n", minishell->exit_code);
	terminal_print("", 1, STDOUT_FILENO);
	ft_tabfree(input);
	return (0);
}
