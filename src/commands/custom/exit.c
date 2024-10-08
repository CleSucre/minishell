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
	if (cmd->argc > 1)
		status = ft_atoi(cmd->args[1]);
	return (status % 256);
}
