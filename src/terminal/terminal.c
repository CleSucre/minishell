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
 * @brief Clear the lines and put back prompt after moving cursor
 *
 * @param input
 * @param cols
 */
static void	reset_stdin(t_minishell *minishell, const char *input)
{
	(void)input;
	ft_putstr_fd("\033[2K", 1);
	move_cursor_back(minishell->term->cols);
	terminal_print(minishell->cache->prompt, 0);
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
	char		*cmd;

	cmd = NULL;
	new_history = NULL;
	if (read(STDIN_FILENO, &seq[0], 1) == -1)
		return (-1);
	if (read(STDIN_FILENO, &seq[1], 1) == -1)
		return (-1);
	if (seq[0] == '[')
	{
		if (seq[1] == 'A')
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
		else if (seq[1] == 'B')
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
		else if (seq[1] == 'C' && cols
			< ft_strlen(*input) + minishell->cache->prompt_len + 1)
			ft_putstr_fd("\033[1C", 1);
		else if (seq[1] == 'D' && cols > minishell->cache->prompt_len + 1)
			ft_putstr_fd("\033[1D", 1);
		return (1);
	}
	return (0);
}

/**
 * @brief Add a char in string at "cols" (n) position
 * 			and put back the cursor at the right place
 *
 * @param char *input	string to add char
 * @param char c		char to add
 * @param cols			Position to add char
 * @return
 */

char	*put_in_string(t_minishell *minishell, char *input, char c)
{
	char	*res;
	size_t	i;
	int		cols;

	cols = minishell->term->cols;
	res = ft_calloc(ft_strlen(input) + 1, sizeof(char *));
	i = 0;
	ft_putstr_fd("\033[s", 1);
	while (input[i] && i < cols - minishell->cache->prompt_len - 1)
	{
		res[i] = input[i];
		i++;
	}
	res[i++] = c;
	while (input[i - 1])
	{
		res[i] = input[i - 1];
		i++;
	}
	free(input);
	reset_stdin(minishell, res);
	terminal_print(res, 0);
	ft_putstr_fd("\033[u\033[1C", 1);
	return (res);
}

/**
 * @brief Delete a char in string at "cols" (n) position
 * 			and put back the cursor at the right place
 *
 * @param char *input	String to delete char
 * @param size_t cols	Position to delete char
 * @return
 */
char	*erase_in_string(t_minishell *minishell, char *input)
{
	char			*res;
	size_t			i;
	unsigned int	cols;

	cols = minishell->term->cols;
	if (cols <= minishell->cache->prompt_len)
		return (input);
	res = ft_calloc(ft_strlen(input), sizeof(char *));
	i = 0;
	ft_putstr_fd("\033[s", 1);
	while (input[i] && i < cols - minishell->cache->prompt_len - 2)
	{
		res[i] = input[i];
		i++;
	}
	i++;
	while (input[i])
	{
		res[i - 1] = input[i];
		i++;
	}
	reset_stdin(minishell, input);
	free(input);
	terminal_print(res, 0);
	ft_putstr_fd("\033[u", 1);
	if (cols > minishell->cache->prompt_len + 1)
		ft_putstr_fd("\033[1D", 1);
	return (res);
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
	while (1)
	{
		get_cursor_position(minishell->term);
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
