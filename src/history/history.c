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
 * @brief Get ministory file fd for reading and writing
 * 			If the file does not exist, it will be created
 *
 * @return int fd if success, -1 if failed
 */
static int	history_get_file(void)
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

/**
 * @brief Get the current history command depending on the history_pos
 *
 * @param t_minishell *minishell
 * @return t_history *
 */
static t_history	*history_get_current(t_minishell *minishell)
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
 * @brief Get the next command in history, depending on history_pos
 * 			Increment history_pos by 1 and return the command
 * 			Return current command if history_pos is already at the end
 *
 * @param t_minishell *minishell
 * @return t_history *
 */
static t_history	*history_up(t_minishell *minishell)
{
	if (minishell->history_pos < minishell->history_size)
		minishell->history_pos++;
	return (history_get_current(minishell));
}

/**
 * @brief Get the previous command in history, depending on history_pos
 * 			Decrement history_pos by 1 and return the command
 * 			Return current command if history_pos is already at the beginning
 * @param t_minishell *minishell
 * @return t_history *
 */
static t_history	*history_down(t_minishell *minishell)
{
	if (minishell->history_pos > 0)
		minishell->history_pos--;
	return (history_get_current(minishell));
}

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

	if (ft_strlen(cmd) == 0)
		return (history_up(minishell));
	pos = minishell->history_pos;
	history = history_up(minishell);
	if (!history)
		return (history_get_current(minishell));
	while (history && history->cmd && cmd
		&& minishell->history_pos < minishell->history_size)
	{
		if (ft_strncmp(history->cmd, cmd, ft_strlen(cmd)) == 0
			&& pos != minishell->history_pos)
			return (history);
		history = history_up(minishell);
	}
	minishell->history_pos = pos;
	return (history_get_current(minishell));
}

/**
 * @brief Search in history a command that start with cmd
 * 			Return the next one depending on the history_pos
 * 			Return the current command if no match is found
 *
 * @param t_minishell *minishell
 * @param char *cmd
 * @return t_history *
 */
t_history	*history_find_down(t_minishell *minishell, char *cmd)
{
	t_history		*history;
	unsigned int	pos;

	if (ft_strlen(cmd) == 0)
		return (history_down(minishell));
	pos = minishell->history_pos;
	history = history_down(minishell);
	if (!history)
		return (history_get_current(minishell));
	while (history && history->cmd && cmd)
	{
		if (ft_strncmp(history->cmd, cmd, ft_strlen(cmd)) == 0
			&& pos != minishell->history_pos)
			return (history);
		history = history_down(minishell);
	}
	return (history_get_current(minishell));
}

/**
 * @brief Load the history from file to memory
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
	debug_history_loaded(minishell->history_size);
	close(fd);
	return (0);
}

/**
 * @brief Add command to history file and memory
 * 			If the command is the same as the last one, it will not be added
 * 			to memory but will still be added to the file for log purposes
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

	if (!minishell->history->older || !minishell->history->older->cmd
		|| ft_strcmp(minishell->history->older->cmd, cmd) != 0)
	{
		new = ft_calloc(1, sizeof(t_history));
		if (!new)
			return (-1);
		new->cmd = ft_strdup(cmd);
		new->newer = minishell->history;
		new->older = minishell->history->older;
		if (minishell->history->older)
			minishell->history->older->newer = new;
		minishell->history->older = new;
		minishell->history_size++;
		debug_history_add(cmd, 1);
	}
	else
		debug_history_add(cmd, 0);
	if (!fs)
		return (0);
	fd = history_get_file();
	if (fd < 0)
		return (-1);
	ft_putstr_fd(cmd, fd);
	ft_putchar_fd('\n', fd);
	debug_history_add_file(cmd);
	close(fd);
	return (0);
}

/**
 * @brief Print the history in the terminal
 *
 * @param t_minishell *minishell
 * @return void
 */
void	history_print(t_minishell *minishell)
{
	t_history		*history;
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
	terminal_print(minishell->cache->prompt, 1);
}

/**
 * @brief Reset the history to the initial state
 *
 * TODO: Do we need this function?
 *
 * @param None
 * @return void
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
