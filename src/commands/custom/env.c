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

/**
 * @brief Print the environment variables
 *
 * @param t_cmd *cmd Command structure
 * @return int Exit code
 */
int	command_env(t_cmd *cmd)
{
	int			i;

	if (contain_flag(cmd->args, 'i'))
	{
		terminal_print("", 1, cmd->output_fd);
		return (0);
	}
	else if (cmd->argc > 1)
	{
		terminal_print("env: ‘", 0, cmd->output_fd);
		terminal_print(cmd->args[1], 0, cmd->output_fd);
		terminal_print("’: No such file or directory", 0, cmd->output_fd);
	}
	else
	{
		i = 0;
		while (cmd->env[i])
		{
			terminal_print(cmd->env[i], 1, cmd->output_fd);
			i++;
		}
	}
	terminal_print("", 1, cmd->output_fd);
	return (0);
}
