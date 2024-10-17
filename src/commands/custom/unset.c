/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
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
	char	*arg_equal;
	int		position;

	cut_name = ft_split_quote(cmd->args[1], "+", "\"\'");
	if (!cut_name)
		return (1);
	arg_equal = ft_strjoin(cut_name[0], "=");
	if (!arg_equal)
	{
		ft_tabfree(cut_name);
		return (1);
	}
	position = find_table_args(cmd->env, arg_equal);
	if (position != -1)
		ft_tabdel(cmd->env, position);
	ft_tabfree(cut_name);
	free(arg_equal);
	return (0);
}
