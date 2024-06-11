/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 05:39:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/06/03 05:39:00 by mpierrot         ###   ########.fr       */
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
int	interpret_escape_sequence(t_minishell *minishell, char **input, size_t cols)
{
	char		seq[2];
	t_history	*new_history;
	char		*cmd;

	cmd = NULL;
	new_history = NULL;
	if (read(STDIN_FILENO, &seq[0], 1) == -1)
		return (-1);
	if (read(STDIN_FILENO, &seq[1], 1) == -1)
		return (-1);
	if (seq[0] == '[')
	{
		if (seq[1] == U_ARROW)
		{
			if (minishell->history_pos == 0)
			{
				free(minishell->cache->input);
				minishell->cache->input = ft_strdup(*input);
			}
			new_history = history_find_up(minishell, minishell->cache->input);
			if (new_history && new_history->cmd)
			{
				free(*input);
				*input = ft_strdup(new_history->cmd);
				ft_putstr_fd("\033[1000D", 1);
				terminal_print("\033[2K", 0);
				terminal_print(minishell->cache->prompt, 0);
				terminal_print(*input, 0);
			}
		}
		else if (seq[1] == D_ARROW)
		{
			if (minishell->history_pos == 0)
			{
				free(minishell->cache->input);
				minishell->cache->input = ft_strdup(*input);
			}
			new_history = history_find_down(minishell, minishell->cache->input);
			if (new_history && new_history->cmd)
				cmd = ft_strdup(new_history->cmd);
			else
				cmd = ft_strdup(minishell->cache->input);
			free(*input);
			*input = ft_strdup(cmd);
			free(cmd);
			ft_putstr_fd("\033[1000D", 1);
			terminal_print("\033[2K", 0);
			terminal_print(minishell->cache->prompt, 0);
			terminal_print(*input, 0);
		}
		else if (seq[1] == R_ARROW && cols
			< ft_strlen(*input) + minishell->cache->prompt_len + 1)
		{
			minishell->term->cols++;
			ft_putstr_fd("\033[1C", 1);
		}
		else if (seq[1] == L_ARROW && cols > minishell->cache->prompt_len + 1)
		{
			minishell->term->cols--;
			ft_putstr_fd("\033[1D", 1);
		}
		return (1);
	}
	return (0);
}

/**
 * @brief Use termios to get input from user and process it
 *
 * @param t_minishell *minishell
 * @return int 0 if no error, 1 if error
 */

int	use_termios(t_minishell *minishell)
{
	char	*input;
	char	c;

	input = NULL;
	reset_input(&input);
	terminal_print(minishell->cache->prompt, 1);
	get_cursor_position(minishell->term);
	while (1)
	{
		if (read(STDIN_FILENO, &c, 1) == -1)
		{
			perror("read");
			return (1);
		}
		if (process_action(minishell, c, &input))
			break ;
	}
	terminal_print(TERMINAL_EXIT_MSG, 1);
	terminal_print("", 1);
	free(input);
	return (0);
}
