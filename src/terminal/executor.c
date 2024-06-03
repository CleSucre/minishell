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
	(void)minishell;
	if (DEBUG)
	{
		terminal_print(BOLDWHITE"[DEBUG] command: "RESET, 1);
		terminal_print(input, 0);
	}
	if (ft_isprint(*input))
	{
		//add_to_history(minishell, input);
	}
	if (ft_strncmp(input, "exit", 4) == 0)
		return (1);
	return (0);
}
