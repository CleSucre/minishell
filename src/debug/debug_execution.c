/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_debug.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Print the command executed in debug mode
 *
 * @param cmd
 * @return void
 */
void	debug_execution(char *cmd)
{
	if (!DEBUG)
		return ;
	terminal_print(BOLDBLUE"[DEBUG] "RESET"Command "BOLDYELLOW, 1);
	terminal_print(cmd, 0);
	terminal_print(RESET" executed", 0);
}
