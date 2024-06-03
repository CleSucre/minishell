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
char	*search_history(t_history *history, char *cmd, int direction)
{
	t_history	*tmp;
	int 		i;

	tmp = history;
	i = 0;
	if (direction == 1)
	{
		while (tmp->newer)
		{
			tmp = tmp->newer;
			if (i++ > history->pos)
				continue ;
			else if (ft_strncmp(tmp->cmd, cmd, ft_strlen(tmp->cmd)) == 0)
			{
				history->pos++;
				continue ;
			}
			if (ft_strncmp(tmp->cmd, cmd, ft_strlen(cmd)) == 0)
				return (tmp->cmd);
			history->pos++;
		}
	}
	return (NULL);
}

/**
 * @brief Load the history from file
 *
 * @param t_history *history
 * @return int 0 if success, -1 if failed
 */
int	load_history(t_history *history)
{
	int		fd;
	char	*line;
	char	*trimmed;

	fd = open(HISTORY_FILE, O_RDONLY);
	if (fd < 0)
	{
		if (DEBUG)
			ft_fprintf(2, BOLDRED"Error: "RESET"open failed\n");
		return (-1);
	}
	line = get_next_line(fd);
	while (line)
	{
		trimmed = ft_strtrim(line, WHITESPACES);
		if (trimmed && *trimmed)
			add_to_history(history, trimmed, 0);
		free(trimmed);
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
int	add_to_history(t_history *history, char *cmd, int fs)
{
	t_history	*new;
	int			fd;

	if (history->newer && history->newer->cmd && ft_strcmp(history->newer->cmd, cmd) == 0)
	{
		if (DEBUG)
		{
			terminal_print(BOLDWHITE"[DEBUG] "RESET"Command "BOLDWHITE, 1);
			terminal_print(cmd, 0);
			terminal_print(RESET" not added to history (already the last command)", 0);
		}
	} else {
		new = malloc(sizeof(t_history));
		if (!new)
			return (-1);
		/*
		 * typedef struct s_history
		 * {
		 * char				*cmd;
		 * struct s_history	*older;
		 * struct s_history	*newer;
		 * }		t_history;
		 */
		new->cmd = ft_strdup(cmd);
		new->older = history;
		new->newer = history->newer;
		if (history->newer)
			history->newer->older = new;
		history->newer = new;
		if (DEBUG)
		{
			terminal_print(BOLDWHITE"[DEBUG] "RESET"Command "BOLDWHITE, 1);
			terminal_print(cmd, 0);
			terminal_print(RESET" added to history", 0);
		}
	}
	if (!fs)
		return (0);
	fd = get_history_file();
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

void	free_history(t_history *history)
{
	t_history	*tmp;

	while (history)
	{
		tmp = history;
		history = history->newer;
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
void	reset_history(void)
{
	int	trunc;

	trunc = open(HISTORY_FILE, O_WRONLY | O_TRUNC);
	if (trunc == -1)
	{
		if (DEBUG)
			ft_fprintf(2, BOLDRED"Error: "RESET"open failed\n");
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
int	get_history_file(void)
{
	int	fd;

	fd = open(HISTORY_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
	{
		if (DEBUG)
			ft_fprintf(2, BOLDRED"Error: "RESET"open failed\n");
		return (-1);
	}
	return (fd);
}
