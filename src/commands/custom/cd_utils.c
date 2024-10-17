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
	char	**tmp;

	if (find_table_args(minishell->env, "OLDPWD") != -1)
		return (0);
	tmp = ft_tabdup((const char **)cmd->env);
	add_cmd_env(minishell, "OLDPWD", getcwd(NULL, 0));
	cmd->env = ft_tabinsert(tmp, ft_strjoin("OLDPWD=", getcwd(NULL, 0)),
			ft_tablen((const char **)cmd->env) - 1);
	return (1);
}
