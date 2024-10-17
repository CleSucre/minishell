/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 05:57:17 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/17 06:03:44 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_oldpwd(t_minishell *minishell, t_cmd *cmd)
{
	char	buffer[BUFFER_SIZE];

	if (find_table_args(cmd->env, "OLDPWD") != -1)
		return (0);
	add_cmd_env(minishell, "OLDPWD", getcwd(buffer, BUFFER_SIZE));
	cmd->env = minishell->env;
	return (1);
}
