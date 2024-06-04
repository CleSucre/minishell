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
 * @brief Search in history list for a command with the same prefix
 * starting from the given index
 *
 * @param t_history *history
 * @param char *cmd
 * @param int direction 1 for newer, -1 for older
 * @return char *
 */
t_history	*history_find(t_minishell *minishell, char *cmd, int direction)
{
	t_history	*history;
	int 		i;

	history = minishell->history;
	i = 0;
	if (direction == 1)
	{
		while (history->newer)
		{
			history = history->newer;
			if (i < history->pos)
			{
				i++;
				continue ;
			}
			if (ft_strncmp(history->cmd, cmd, ft_strlen(cmd)) == 0)
			{
				history->pos = i;
				return (history);
			}
			i++;
		}
	}
	return (NULL);
}

t_history	*history_up(t_minishell *minishell)
{
	if (minishell->history_pos < minishell->history_size)
		minishell->history_pos++;
	return (history_get_current(minishell));
}

t_history	*history_down(t_minishell *minishell)
{
	if (minishell->history_pos > 0)
		minishell->history_pos--;
	return (history_get_current(minishell));
}

t_history *history_get_current(t_minishell *minishell)
{
	int 		i;
	t_history	*history;

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
	{
		if (DEBUG)
			ft_fprintf(2, BOLDRED"Error: "RESET""HISTORY_FILE" open failed\n");
		return (-1);
	}
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

	if (minishell->history->newer && minishell->history->newer->cmd && ft_strcmp(minishell->history->newer->cmd, cmd) == 0)
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
		if (DEBUG)
			ft_fprintf(2, BOLDRED"Error: "RESET""HISTORY_FILE" open failed\n");
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
		if (DEBUG)
			ft_fprintf(2, BOLDRED"Error: "RESET""HISTORY_FILE" open failed\n");
		return (-1);
	}
	return (fd);
}
