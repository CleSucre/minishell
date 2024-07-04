/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Print the number of custom loaded in debug mode
 *
 * @param int cmd_count
 * @return void
 */
void	debug_history_loaded(int cmd_count)
{
	char	*count;

	if (!DEBUG)
		return ;
	count = ft_itoa(cmd_count);
	terminal_print(BOLDBLUE"[DEBUG] "RESET"History loaded with "BOLDYELLOW, 1);
	terminal_print(count, 0);
	terminal_print(RESET" custom", 0);
	free(count);
}

/**
 * @brief Print the command added to history in debug mode
 *
 * @param char *cmd
 * @param int added 1 if the command was added, 0 otherwise
 * @return void
 */
void	debug_history_add(char *cmd, int added)
{
	if (!DEBUG)
		return ;
	if (added)
	{
		terminal_print(BOLDBLUE"[DEBUG] "RESET"Command "BOLDYELLOW, 1);
		terminal_print(cmd, 0);
		terminal_print(RESET" added to history", 0);
	}
	else
	{
		terminal_print(BOLDBLUE"[DEBUG] "RESET"Command "BOLDYELLOW, 1);
		terminal_print(cmd, 0);
		terminal_print(RESET" not added to history "
			"(already the last command)", 0);
	}
}

/**
 * @brief Print the command added to history file in debug mode
 *
 * @param char *cmd
 * @return void
 */
void	debug_history_add_file(char *cmd)
{
	if (!DEBUG)
		return ;
	terminal_print(BOLDBLUE"[DEBUG] "RESET"Command "BOLDYELLOW, 1);
	terminal_print(cmd, 0);
	terminal_print(RESET" added to history file", 0);
}
