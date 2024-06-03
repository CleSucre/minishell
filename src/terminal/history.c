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
 * @param int index
 * @return char *
 */
char	*search_history(t_history *history, char *cmd)
{
	t_history	*tmp;

	tmp = history;
	while (tmp && tmp->next)
	{
		ft_fprintf(2, "[%s] %d\n", cmd, ft_strlen(cmd));
		if (ft_strncmp(tmp->cmd, cmd, ft_strlen(cmd)) == 0)
		{
			return (tmp->cmd);
		}
		tmp = tmp->next;
	}
	return ("NONE");
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
		add_to_history(history, line, 0);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}

/**
 * @brief Add command to history file
 *
 * @param t_history *history
 * @param char *cmd
 * @param int fs save to file if 1
 * @return none
 */
void	add_to_history(t_history *history, char *cmd, int fs)
{
	t_history	*new;
	int			fd;

	new = malloc(sizeof(t_history));
	if (!new)
		return ;
	new->cmd = ft_strdup(cmd);
	new->next = history->next;
	new->prev = history;
	history->next = new;
	if (DEBUG)
	{
		terminal_print(BOLDWHITE"[DEBUG] "RESET"Command "BOLDWHITE, 1);
		terminal_print(new->cmd, 0);
		terminal_print(RESET" added to history", 0);
	}
	if (!fs)
		return ;
	fd = get_history_file();
	if (fd < 0)
		return ;
	ft_putstr_fd(new->cmd, fd);
	ft_putchar_fd('\n', fd);
}

void	free_history(t_history *history)
{
	t_history	*tmp;

	while (history)
	{
		tmp = history;
		history = history->next;
		free(tmp->cmd);
		free(tmp);
	}
	free(history);
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
