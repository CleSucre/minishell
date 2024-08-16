/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:24:43 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 09:51:15 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin_command(t_minishell *minishell, t_cmd *cmd)
{
	if (ft_strcmp(cmd->cmd_name, "cd") == 0)
		command_cd(cmd);
	else if (ft_strcmp(cmd->cmd_name, "echo") == 0)
		command_echo(cmd);
	else if (ft_strcmp(cmd->cmd_name, "env") == 0)
		command_env(cmd);
	else if (ft_strcmp(cmd->cmd_name, "exit") == 0)
	{
		command_exit(cmd);
		return (2);
	}
	else if (ft_strcmp(cmd->cmd_name, "export") == 0)
		command_export(cmd, minishell);
	else if (ft_strcmp(cmd->cmd_name, "history") == 0)
		command_history(cmd, minishell);
	else if (ft_strcmp(cmd->cmd_name, "pwd") == 0)
		command_pwd(cmd);
	else if (ft_strcmp(cmd->cmd_name, "unset") == 0)
		command_unset(cmd, minishell);
	return (0);
}

int is_builtin_command(t_cmd *cmd)
{
	if (ft_strcmp(cmd->cmd_name, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_name, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_name, "env") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_name, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_name, "export") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_name, "history") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_name, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_name, "unset") == 0)
		return (1);
	return (0);
}