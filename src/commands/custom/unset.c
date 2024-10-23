/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/10 14:20:34 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Unset the environment variable
 *
 * @param t_cmd *cmd Command structure
 * @return int Exit code
 */
int	command_unset(t_cmd *cmd)
{
	char	**cut_name;
	int		position;
	int		i;

	if (!cmd->args[1])
		return (0);
	cut_name = ft_split_quote(cmd->args[1], " ", "\"\'");
	if (!cut_name)
		return (1);
	i = 1;
	while (cmd->args[i])
	{
		position = find_table_args(cmd->env, cmd->args[i]);
		if (position != -1)
			ft_tabdel(cmd->env, position);
		i++;
	}
	ft_tabfree(cut_name);
	return (0);
}
