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
		terminal_print("", 1);
		return ;
	}
	else if (cmd->argc > 1)
	{
		terminal_print("env: ‘", 0);
		terminal_print(cmd->argv[1], 0);
		terminal_print("’: No such file or directory", 0);
	}
	else
	{
		i = 0;
		while (cmd->env[i])
		{
			terminal_print(cmd->env[i], 1);
			i++;
		}
	}
	terminal_print("", 1);
}
