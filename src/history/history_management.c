/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 03:54:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/12 03:54:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Create a new history element
 *
 * @param char *cmd
 * @param t_history *older
 * @param t_history *newer
 * @return t_history *
 */
static t_history	*history_create(char *cmd, t_history *older,
							t_history *newer)
{
	t_history	*history;

	history = (t_history *) ft_calloc(1, sizeof(t_history));
	if (!history)
		return (NULL);
	history->cmd = cmd;
	history->older = older;
	history->newer = newer;
	return (history);
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
		new = history_create(ft_strdup(cmd),
				minishell->history->older, minishell->history);
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
	ft_fprintf(fd, "%d\n", cmd);
	debug_history_add_file(cmd);
	close(fd);
	return (0);
}

/**
 * @brief Reset the history file to the initial state
 *
 * TODO: Do we need this function?
 *
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

/**
 * @brief Print the content of history file to the terminal
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
	print_terminal_prompt(minishell, 1);
}
