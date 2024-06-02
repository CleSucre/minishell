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

#include "../../include/minishell.h"

/**
 * @brief Reset the history & delete the history file
 *
 * @param none
 * @return none
 */
void	reset_history(void)
{
	int	fd;
	int	trunc;

	fd = get_history_file();
	trunc = open(HISTORY_FILE, O_WRONLY | O_TRUNC);
	if (trunc == -1)
	{
		if (DEBUG)
			ft_fprintf(2, BOLDRED"Error: "RESET"open failed\n");
		close(fd);
		return ;
	}
	close (fd);
	close (trunc);
}

/**
 * @brief Add command to history file
 *
 * @param char *cmd
 * @return none
 */
void	add_to_history(char *cmd)
{
	int	fd;

	fd = get_history_file();
	if (fd < 0)
	{
		if (DEBUG)
			ft_fprintf(2, BOLDWHITE"[DEBUG] "
				BOLDRED"Error: "RESET"open failed\n");
		return ;
	}
	write(fd, cmd, ft_strlen(cmd));
	write(fd, "\n", 1);
	if (DEBUG)
		ft_printf(BOLDWHITE"[DEBUG] "RESET"Command "
			BOLDWHITE"%s"RESET" added to history\n", cmd);
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
