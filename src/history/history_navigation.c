/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_navigation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 03:54:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/12 03:54:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get the current history command depending on the history_pos
 *
 * @param t_minishell *minishell Minishell structure
 * @return t_history * Current history command
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
 * @param t_minishell *minishell Minishell structure
 * @return t_history * Next history command
 */
static t_history	*history_up(t_minishell *minishell)
{
	if (minishell->history_pos <= minishell->history_size)
		minishell->history_pos++;
	return (history_get_current(minishell));
}

/**
 * @brief Get the previous command in history, depending on history_pos
 * 			Decrement history_pos by 1 and return the command
 * 			Return current command if history_pos is already at the beginning
 *
 * @param t_minishell *minishell Minishell structure
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
 * @param t_minishell *minishell Minishell structure
 * @param char *cmd Command to search
 * @return t_history * History command
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
		&& minishell->history_pos <= minishell->history_size)
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
