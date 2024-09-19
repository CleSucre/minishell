/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 03:54:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/12 03:54:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Create ministory file fd at the beginning
 *
 * @return int fd if success, -1 if failed
 */
int	history_create_file(void)
{
	int	fd;

	fd = open(HISTORY_FILE, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
	{
		terminal_print(BOLDRED"Error: "RESET""HISTORY_FILE" open failed",
			1, STDOUT_FILENO);
		return (-1);
	}
	return (fd);
}

/**
 * @brief Get ministory file fd for reading and writing
 * 			If the file does not exist, it will be created
 *
 * @return int fd if success, -1 if failed
 */
int	history_goto_file(t_minishell *minishell)
{
	int		fd;
	char	*name;

	name = ft_strjoin(minishell->starting_path, "/.ministory");
	if (!name)
		return (-1);
	fd = open(name, O_RDWR | O_APPEND | O_CREAT, 0644);
	free(name);
	if (fd < 0)
	{
		terminal_print(BOLDRED"Error: "RESET""HISTORY_FILE" open failed",
			1, STDOUT_FILENO);
		return (-1);
	}
	return (fd);
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

	fd = history_create_file();
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
