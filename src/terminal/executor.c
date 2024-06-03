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

int	is_valide(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] <= 32 || str[i] >= 127)
			return (0);
		i++;
	}
	return (1);
}

int	exec_command(t_minishell *minishell, char *input)
{
	(void)minishell;
	if (DEBUG)
	{
		terminal_print(BOLDWHITE"[DEBUG] command: "RESET, 1);
		terminal_print(input, 0);
	}
	/*
	if (is_valide(input))
		add_to_history(minishell, input);
	 */
	if (ft_strncmp(input, "exit", 4) == 0)
		return (1);
	return (0);
}
