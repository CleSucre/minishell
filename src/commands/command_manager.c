/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:24:43 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/10 09:13:33 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_custom_command(t_minishell *minishell, t_cmd *cmd)
{
	if (ft_strcmp(cmd->cmd_name, "env") == 0)
		command_env(cmd);
	else if (ft_strcmp(cmd->cmd_name, "exit") == 0)
	{
		command_exit(cmd);
		return (2);
	}
	else if (ft_strcmp(cmd->cmd_name, "history") == 0)
		command_history(cmd, minishell);
	else if (ft_strcmp(cmd->cmd_name, "echo") == 0)
		command_echo(cmd);
	else if (ft_strcmp(cmd->cmd_name, "cd") == 0)
		command_cd(cmd);
	else
		return (0);
	return (1);
}
