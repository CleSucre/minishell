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
 * @brief	Move back cursor to fake an erasure in our term
 * @param len
 */

void	erase_term(size_t len)
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
 * @brief 	Print in our terminal
 * 			if nl, move cursor one line down
 * @param str 	String to print
 * @param nl	Move cursor down and print newline(s)
 */
void	terminal_print(char *str, int nl)
{
	if (nl)
		ft_printf("\033[1000D", str);
	while (nl--)
		ft_putchar_fd('\n', 1);
	ft_printf("%s", str);
}


void	reset_input(char **input)
{
	if (*input) {
		free(*input);
		*input = NULL;
	}
	*input = ft_calloc(sizeof(char *), 1);
//	(*input)[0] = '\0';
}

/**
 * @brief Get cursor position in our terminal
 * 			Ask for it
 * 			Read terms answer
 * 			Analyse it
 * @param rows position y
 * @param cols position x
 */

void	get_cursor_position(int *rows, size_t *cols)
{
	char			buf[32];
	unsigned int	i;
	int				ret;

	i = 0;
	write(STDOUT_FILENO, "\033[6n", 4);
	while (i < sizeof(buf) - 1)
	{
		ret = read(STDIN_FILENO, buf + i, 1);
		if (ret != 1 || buf[i] == 'R')
			break ;
		i++;
	}
	buf[i] = '\0';
	if (buf[0] == '\033' && buf[1] == '[')
	{
		i = 2;
		*rows = ft_atoi(buf + i);
		while (buf[i] && ft_isdigit(buf[i]))
			i++;
		*cols = ft_atoi(buf + i + 1);
	}
}

/**
 * @brief Specifically for escape sequence as up-down-left-right arrow
 * 				- Left & right : Move cursor left and right as bash
 * 				- Up & Down : 	go search familiar input user already wrote in .ministory
 * @param history 	.ministory
 * @param input 	String with command catch by term
 * @param cols		position
 * @param seq[2]	read stdin and sort which input was done
 * @return
 */

int	interpret_escape_sequence(t_minishell *minishell, char **input, size_t cols)
{
	char		seq[2];
	t_history	*new_history;

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
				if (minishell->cache->input)
					free(minishell->cache->input);
				minishell->cache->input = ft_strdup(*input);
			}
			if (!minishell->cache->input)
				new_history = history_find_up(minishell, *input);
			else if (ft_strlen(*input))
			{
				new_history = history_find_up(minishell, minishell->cache->input);
				if (!new_history)
					new_history = history_get_current(minishell);
			}
			if (!new_history)
				new_history = history_up(minishell);
			if (new_history && new_history->cmd)
			{
				erase_term(ft_strlen(*input));
				free(*input);
				*input = ft_strdup(new_history->cmd);
				ft_putstr_fd("\033[1000D", 1);
				terminal_print("\033[2K", 0);
				terminal_print(TERMINAL_PROMPT, 0);
				terminal_print(*input, 0);
			}
		}
		else if (seq[1] == 'B')
		{
			if (minishell->cache->input)
			{
				new_history = history_find_down(minishell, minishell->cache->input);
				if (!new_history)
				{
					free(*input);
					*input = ft_strdup(minishell->cache->input);
					ft_putstr_fd("\033[1000D", 1);
					terminal_print("\033[2K", 0);
					terminal_print(TERMINAL_PROMPT, 0);
					terminal_print(*input, 0);
					return (1);
				}
			}
			else
				new_history = history_down(minishell);
			if (new_history && new_history->cmd)
			{
				free(*input);
				*input = ft_strdup(new_history->cmd);
				ft_putstr_fd("\033[1000D", 1);
				terminal_print("\033[2K", 0);
				terminal_print(TERMINAL_PROMPT, 0);
				terminal_print(*input, 0);
			}
		}
		else if (seq[1] == 'C' && cols < ft_strlen(*input) + ft_strlen(TERMINAL_PROMPT) + 1)
			ft_putstr_fd("\033[1C", 1);
		else if (seq[1] == 'D' && cols > ft_strlen(TERMINAL_PROMPT) + 1)
			ft_putstr_fd("\033[1D", 1);
		return (1);
	}
	return (0);
}


/**
 * @brief 		Sort inputs && act in consequence
 * @param minishell struct which access history
 * @param c 		char read by use_termios
 * @param input 	string which join every char read by termios from 1st to Enter
 * @param cols 		Cursor position
 * @return
 */

/**
 * @brief Move cursor from cols to n positions
 * @param position
 */
void	move_cursor_back(int position)
{
	int i;
	i = 0;
	while(i++ < position)
		ft_putstr_fd("\033[1D", 1);
}

/**
 * @brief Clear the lines and put back prompt after moving cursor
 * @param input
 * @param cols
 */
void	reset_stdin(char *input, int cols)
{
	(void)input;
	ft_putstr_fd("\033[2K", 1);
	move_cursor_back(cols);
	terminal_print(TERMINAL_PROMPT, 0);
}

/**
 * @brief 	Add a char in string at "cols" (n) position
 * 			and put back the cursor at the right place
 * @param input
 * @param cols
 * @return
 */

char	*put_in_string(char *input, char c, size_t cols)
{
	char	*res;
	size_t	i;

	res = ft_calloc(sizeof(char *) * ft_strlen(input) + 1, 1);
	i = 0;
	ft_putstr_fd("\033[s", 1);
	while (input[i] && i < cols - ft_strlen(TERMINAL_PROMPT) - 1)
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
	reset_stdin(res, cols);;
	terminal_print(res, 0);
	ft_putstr_fd("\033[u\033[1C", 1);
	return (res);
}


/**
 * @brief 	Delete a char in string at "cols" (n) position
 * 			and put back the cursor at the right place
 * @param input
 * @param cols
 * @return
 */

char	*erase_in_string(char *input, size_t cols)
{
	char	*res;
	size_t	i;

	if (cols <= ft_strlen(TERMINAL_PROMPT))
		return (input);
	res = ft_calloc(sizeof(char *) * ft_strlen(input), 1);
	i = 0;
	ft_putstr_fd("\033[s", 1);
	while (input[i] && i < cols - ft_strlen(TERMINAL_PROMPT) - 2)
	{
		res[i] = input[i];
		i++;
	}
	i++;
	while (input[i])
	{
		res[i-1] = input[i];
		i++;
	}
	free(input);
	reset_stdin(input, cols);
	terminal_print(res, 0);
	ft_putstr_fd("\033[u", 1);
	if (cols > ft_strlen(TERMINAL_PROMPT) + 1)
		ft_putstr_fd("\033[1D", 1);
	return (res);
}

int	process_action(t_minishell *minishell, char c, char **input, size_t cols)
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
		if (ft_strlen(*input) > 0 && cols != ft_strlen(TERMINAL_PROMPT) + ft_strlen(*input) + 1)
			*input = erase_in_string(*input, cols);
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
		if (interpret_escape_sequence(minishell, input, cols))
			return (0);
	}
	else
	{
		if (cols != ft_strlen(TERMINAL_PROMPT) + ft_strlen(*input) + 1)
			*input = put_in_string(*input, c, cols);
		else
		{
			*input = ft_charjoin(*input, c);
			ft_putchar_fd(c, 1);
		}
	}
	return (0);
}

/**
 * @brief 	get cursor position (cols && rows)
 *			read inputs and send it to process_action
 *			When while is break, print Goodbye and close program.
 * @param minishell Struct which access to history
 * @return
 */

int	use_termios(t_minishell *minishell)
{
	char	*input;
	char	c;
	int		rows;
	size_t		cols;

	cols = 0;
	input = NULL;
	reset_input(&input);
	terminal_print(TERMINAL_PROMPT, 1);
	while (1)
	{
		get_cursor_position(&rows, &cols);
		if (read(STDIN_FILENO, &c, 1) == -1)
		{
			perror("read");
			return (1);
		}
		if (process_action(minishell, c, &input, cols))
			break ;
	}
	terminal_print("Goodbye !", 1);
	terminal_print("", 1);
	free(input);
	return (0);
}
