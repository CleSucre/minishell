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
 * @brief Process action of cursor up
 *
 * @param t_minishell *minishell
 * @param char **input
 */
void	cursor_up_action(t_minishell *minishell,
		char **input, t_history *new_history)
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
		terminal_print("\033[2K", 0, STDOUT_FILENO);
		print_terminal_prompt(minishell, 0);
		terminal_print(*input, 0, STDOUT_FILENO);
	}
	get_cursor_position(minishell->term);
}

/**
 * @brief Process action of cursor down
 *
 * @param t_minishell *minishell
 * @param char **input
 */
void	cursor_down_action(t_minishell *minishell,
			char **input, t_history *new_history)
{
	char	*cmd;

	cmd = NULL;
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
	terminal_print("\033[2K", 0, STDOUT_FILENO);
	print_terminal_prompt(minishell, 0);
	terminal_print(*input, 0, STDOUT_FILENO);
	get_cursor_position(minishell->term);
}

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

	new_history = NULL;
	if (read(STDIN_FILENO, &seq[0], 1) == -1
		|| read(STDIN_FILENO, &seq[1], 1) == -1)
		return (-1);
	if (seq[0] == '[')
	{
		if (seq[1] == U_ARROW)
			arrow_up_action(minishell, input, new_history);
		else if (seq[1] == D_ARROW)
			arrow_down_action(minishell, input, new_history);
		else if (seq[1] == R_ARROW && cols
			< ft_strlen(*input) + get_prompt_len(minishell) + 1)
			arrow_right_action(minishell);
		else if (seq[1] == L_ARROW && cols > get_prompt_len(minishell) + 1)
			arrow_left_action(minishell);
		return (1);
	}
	return (0);
}

/**
 * @brief Set tabstop every 4 columns
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
	char	*input;
	char	c;

	input = NULL;
	reset_input(&input);
	set_tabstop(minishell);
	creation_dict(minishell);
	print_terminal_prompt(minishell, 1);
	get_cursor_position(minishell->term);
	while (1)
	{
		get_terminal_size(minishell->term);
		minishell->term->begin_rows = ft_strlen(input) % 4294967295;
		if (read(STDIN_FILENO, &c, 1) == -1)
		{
			perror("read");
			return (1);
		}
		if (process_action(minishell, c, &input))
			break ;
	}
	terminal_print(TERMINAL_EXIT_MSG, 1, STDOUT_FILENO);
	terminal_print("", 1, STDOUT_FILENO);
	free(input);
	return (0);
}
