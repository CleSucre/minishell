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
 * @param t_minishell *minishell
 * @param char *cmd
 * @param int index
 * @return char *
 */
char	*search_history(t_minishell *minishell, char *cmd, int index)
{
	t_history	*tmp;
	int			i;

	tmp = minishell->history;
	i = 0;
	while (tmp)
	{
		if (ft_strncmp(tmp->cmd, cmd, ft_strlen(cmd)) == 0)
		{
			if (i == index)
				return (tmp->cmd);
			i++;
		}
		tmp = tmp->next;
	}
	return (NULL);
}

/**
 * @brief Refresh the history list from file
 *
 * @param t_minishell *minishell
 * @return int
 */
int	refresh_history(t_minishell *minishell)
{
	int		fd;
	char	*line;

	fd = open(HISTORY_FILE, O_RDONLY);
	if (fd < 0)
	{
		if (DEBUG)
			ft_fprintf(2, BOLDRED"Error: "RESET"open failed\n");
		return (1);
	}
	line = get_next_line(fd);
	while (line)
	{
		add_to_history(minishell, line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}

/**
 * @brief Add command to history file
 *
 * @param char *cmd
 * @return none
 */
void	add_to_history(t_minishell *minishell, char *cmd)
{
	int			fd;
	t_history	*new;

	new = (t_history *)malloc(sizeof(t_history));
	if (!new)
		return ;
	new->cmd = ft_strdup(cmd);
	new->prev = NULL;
	new->next = minishell->history;
	if (minishell->history)
		minishell->history->prev = new;
	minishell->history = new;
	fd = get_history_file();
	if (fd < 0)
		return ;
	write(fd, cmd, ft_strlen(cmd));
	write(fd, "\n", 1);
	if (DEBUG)
		ft_printf(BOLDWHITE"[DEBUG] "RESET"Command "
			BOLDWHITE"%s"RESET" added to history\n", cmd);
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
