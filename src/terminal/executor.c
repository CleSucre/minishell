/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_command(t_minishell *minishell, char *input)
{
	if (ft_strlen(input) == 0)
		return (0);
	if (DEBUG)
	{
		terminal_print(BOLDWHITE"[DEBUG] "RESET"Command "BOLDWHITE, 1);
		terminal_print(input, 0);
		terminal_print(RESET" executed", 0);
	}
	if (ft_isprint(*input))
		history_add(minishell, input, 1);
	if (ft_strncmp(input, "exit", 4) == 0)
				return (1);
	return (0);
}
