/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 06:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	str_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Exit the shell with a status code (default 0)
 *
 * @param t_cmd *cmd Command structure
 * @return int Exit code
 */
int	command_exit(t_cmd *cmd)
{
	int	status;

	status = 0;
	if (cmd->argc > 2)
	{
		ft_fprintf(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	else if (cmd->argc > 1 && str_is_digit(cmd->args[1]) == 0)
	{
		ft_fprintf(2, "minishell: exit: %s: numeric argument required\n",
			cmd->args[1]);
		cmd->exit_signal = 1;
		return (255);
	}
	else if (cmd->argc > 1)
		status = ft_atoi(cmd->args[1]);
	cmd->exit_signal = 1;
	return (status % 256);
}
