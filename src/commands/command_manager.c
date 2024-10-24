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

/**
 * @brief Execute a builtin command
 *
 * @param t_minishell *minishell
 * @param t_cmd *cmd
 * @return int Return exit code
 */
int	execute_builtin_command(t_minishell *minishell, t_cmd *cmd)
{
	if (ft_strcmp(cmd->name, "cd") == 0)
		return (command_cd(minishell, cmd));
	else if (ft_strcmp(cmd->name, "echo") == 0)
		return (command_echo(cmd));
	else if (ft_strcmp(cmd->name, "env") == 0)
		return (command_env(cmd));
	else if (ft_strcmp(cmd->name, "exit") == 0)
		return (command_exit(cmd, minishell));
	else if (ft_strcmp(cmd->name, "export") == 0)
		return (command_export(minishell, cmd));
	else if (ft_strcmp(cmd->name, "history") == 0)
		return (command_history(cmd, minishell));
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		return (command_pwd(cmd));
	else if (ft_strcmp(cmd->name, "unset") == 0)
		return (command_unset(cmd));
	return (0);
}

/**
 * @brief Check if the command is a builtin command
 *
 * @param t_cmd *cmd
 * @return int Return 1 if the command is a builtin command, 0 otherwise
 */
int	is_builtin_command(t_cmd *cmd)
{
	if (ft_strcmp(cmd->name, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd->name, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd->name, "env") == 0)
		return (1);
	if (ft_strcmp(cmd->name, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd->name, "export") == 0)
		return (1);
	if (ft_strcmp(cmd->name, "history") == 0)
		return (1);
	if (ft_strcmp(cmd->name, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd->name, "unset") == 0)
		return (1);
	return (0);
}
