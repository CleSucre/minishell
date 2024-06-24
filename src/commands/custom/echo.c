/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 06:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_echo(t_cmd *cmd)
{
	int		i;
	int		n_flag;

	i = 1;
	n_flag = 0;
	if (cmd->argc > 1 && ft_strcmp(cmd->argv[1], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	terminal_print("", 1);
	while (i < cmd->argc)
	{
		terminal_print(cmd->argv[i], 0);
		if (i + 1 < cmd->argc)
			ft_putchar_fd(' ', 0);
		i++;
	}
	if (n_flag)
	{
		ft_putstr_fd(BACKGROUND_WHITE, 0);
		ft_putstr_fd(TEXT_BLACK, 0);
		terminal_print("%", 0);
		ft_putstr_fd(RESET, 0);
	}
	terminal_print("", 1);
	return (0);
}
