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
	while (i < cmd->argc)
	{
		terminal_print(cmd->argv[i], 0, STDOUT_FILENO);
		if (i + 1 < cmd->argc)
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (n_flag)
	{
		ft_putstr_fd(BACKGROUND_WHITE, STDOUT_FILENO);
		ft_putstr_fd(TEXT_BLACK, STDOUT_FILENO);
		terminal_print("%", 0, STDOUT_FILENO);
		ft_putstr_fd(RESET, STDOUT_FILENO);
	}
	terminal_print("", 1, STDOUT_FILENO);
	return (0);
}
