/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 06:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO: handle other flags?
void	command_env(t_cmd *cmd)
{
	int			i;

	if (contain_flag(cmd->argv, 'i'))
	{
		terminal_print("", 1, STDOUT_FILENO);
		return ;
	}
	else if (cmd->argc > 1)
	{
		terminal_print("env: ‘", 0, STDOUT_FILENO);
		terminal_print(cmd->argv[1], 0, STDOUT_FILENO);
		terminal_print("’: No such file or directory", 0, STDOUT_FILENO);
	}
	else
	{
		i = 0;
		while (cmd->env[i])
		{
			terminal_print(cmd->env[i], 1, STDOUT_FILENO);
			i++;
		}
	}
	terminal_print("", 1, STDOUT_FILENO);
}
