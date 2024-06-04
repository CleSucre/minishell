/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Search in history a command that start with cmd, return the first one
 *
 * @param t_history *history
 * @param char *cmd
 * @param int direction 1 for newer, -1 for older
 * @return char *
 */
t_history	*history_find_up(t_minishell *minishell, char *cmd)
{
	t_history		*history;
	unsigned int	pos;

	pos = minishell->history_pos;
	history = history_up(minishell);
	while (history && history->cmd && cmd)
	{
		if (ft_strncmp(history->cmd, cmd, ft_strlen(cmd)) == 0)
			return (history);
		history = history_up(minishell);
	}
	minishell->history_pos = pos;
	return (NULL);
}

t_history	*history_find_down(t_minishell *minishell, char *cmd)
{
	t_history		*history;
	unsigned int	pos;

	pos = minishell->history_pos;
	history = history_down(minishell);
	while (history && history->cmd && cmd)
	{
		if (ft_strncmp(history->cmd, cmd, ft_strlen(cmd)) == 0)
			return (history);
		history = history_down(minishell);
	}
	minishell->history_pos = pos;
	return (NULL);
}

t_history	*history_up(t_minishell *minishell)
{
	if (minishell->history_pos < minishell->history_size - 1)
		minishell->history_pos++;
	else
		return (NULL);
	return (history_get_current(minishell));
}

t_history	*history_down(t_minishell *minishell)
{
	if (minishell->history_pos > 0)
		minishell->history_pos--;
	else
		return (NULL);
	return (history_get_current(minishell));
}

t_history *history_get_current(t_minishell *minishell)
{
	unsigned int	i;
	t_history		*history;

	i = 0;
	history = minishell->history;
	while (history)
	{
		if (i == minishell->history_pos)
			return (history);
		history = history->older;
		i++;
	}
	return (NULL);
}

/**
 * @brief Load the history from file
 *
 * @param t_history *history
 * @return int 0 if success, -1 if failed
 */
int	history_load(t_minishell *minishell)
{
	int		fd;
	char	*line;

	fd = history_get_file();
	if (fd < 0)
		return (-1);
	line = get_next_line(fd);
	while (line)
	{
		ft_trunc(&line, 1);
		history_add(minishell, line, 0);
		free(line);
		line = get_next_line(fd);
	}
	if (DEBUG)
		terminal_print(BOLDWHITE"[DEBUG] "RESET"History loaded from file", 1);
	close(fd);
	return (0);
}

/**
 * @brief Add command to history file
 *
 * @param t_history *history
 * @param char *cmd
 * @param int fs save to file if 1
 * @return int 0 if added, -1 if failed
 */
int	history_add(t_minishell *minishell, char *cmd, int fs)
{
	t_history	*new;
	int			fd;

	if (minishell->history->older && minishell->history->older->cmd && ft_strcmp(minishell->history->older->cmd, cmd) == 0)
	{
		if (DEBUG)
		{
			terminal_print(BOLDWHITE"[DEBUG] "RESET"Command "BOLDWHITE, 1);
			terminal_print(cmd, 0);
			terminal_print(RESET" not added to history (already the last command)", 0);
		}
	} else {
		new = ft_calloc(sizeof(t_history), 1);
		if (!new)
			return (-1);
		new->cmd = ft_strdup(cmd);
		new->newer = minishell->history;
		new->older = minishell->history->older;
		if (minishell->history->older)
			minishell->history->older->newer = new;
		minishell->history->older = new;
		minishell->history_size++;
		if (DEBUG)
		{
			terminal_print(BOLDWHITE"[DEBUG] "RESET"Command "BOLDWHITE, 1);
			terminal_print(cmd, 0);
			terminal_print(RESET" added to history", 0);
		}
	}
	if (!fs)
		return (0);
	fd = history_get_file();
	if (fd < 0)
		return (-1);
	ft_putstr_fd(cmd, fd);
	ft_putchar_fd('\n', fd);
	if (DEBUG)
	{
		terminal_print(BOLDWHITE"[DEBUG] "RESET"Command "BOLDWHITE, 1);
		terminal_print(cmd, 0);
		terminal_print(RESET" added to history file", 0);
	}
	close(fd);
	return (0);
}

void	history_print(t_minishell *minishell)
{
	t_history	*history;
	unsigned int	i;
	unsigned int	j;

	history = minishell->history;
	while (history && history->older)
		history = history->older;
	i = 0;
	while (history && history->newer)
	{
		terminal_print("    ", 1);
		terminal_print(ft_itoa(i), 0);
		j = 0;
		while (j < 4 - ft_nbrlen(i))
		{
			terminal_print(" ", 0);
			j++;
		}
		terminal_print(history->cmd, 0);
		history = history->newer;
		i++;
	}
	terminal_print(TERMINAL_PROMPT, 1);
}

void	history_free(t_history *history)
{
	t_history	*tmp;

	while (history)
	{
		tmp = history;
		history = history->older;
		free(tmp->cmd);
		free(tmp);
	}
}

/**
 * @brief Reset the history & delete the history file
 *
 * @param none
 * @return none
 */
void	history_reset(void)
{
	int	trunc;

	trunc = open(HISTORY_FILE, O_WRONLY | O_TRUNC);
	if (trunc == -1)
	{
		terminal_print(BOLDRED"Error: "RESET""HISTORY_FILE" open failed", 1);
		return ;
	}
	close (trunc);
}

/**
 * @brief Create ministory file which is the minishell
 * copy from bash_history and return his fd
 *
 * @param none
 * @return int
 */
int	history_get_file(void)
{
	int	fd;

	fd = open(HISTORY_FILE, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
	{
		terminal_print(BOLDRED"Error: "RESET""HISTORY_FILE" open failed", 1);
		return (-1);
	}
	return (fd);
}
