/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 09:40:45 by julthoma         ###   ########.fr       */
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
	while (i < cmd->argc)
	{
		terminal_print(cmd->argv[i], 0, cmd->output);
		if (i + 1 < cmd->argc)
			ft_putchar_fd(' ', cmd->output);
		i++;
	}
	if (n_flag)
	{
		ft_putstr_fd(BACKGROUND_WHITE, cmd->output);
		ft_putstr_fd(TEXT_BLACK, cmd->output);
		terminal_print("%", 0, cmd->output);
		ft_putstr_fd(RESET, cmd->output);
	}
	terminal_print("", 1, cmd->output);
	return (0);
}
