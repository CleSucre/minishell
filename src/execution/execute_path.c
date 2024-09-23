/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:13:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/09 18:13:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute the command given in input
 *
 * @param t_cmd *cmd
 * @return int error code
 */
int	execute_path(t_cmd *cmd)
{
	int	err;

	err = execve(cmd->path, cmd->args, cmd->env);
	if (err == -1)
		ft_fprintf(STDERR_FILENO, "minishell: command not found: %s\n",
				   cmd->name);
	return (err);
}
