/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 09:45:03 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Export Without args list every exports var in current shell in Abc order
 *
 * export VAR=value
 * export VAR=new_value
 *
 * if already exist in env -> free the args, then change it by new input
 * if not exist -> add it to env
 */

/***
* @brief take input and value, and put them in env
*/
//int	modify_env_input(t_minishell *minishell, char *input, char *value)
//{
//	int		is_here;
//	char	*res;
//
//	is_here = 0;
//	is_here = find_table_args(minishell->env, input);
//	if (is_here == -1)
//		return (-1);
//	ft_printf("AVANT la env : [%s]\n", minishell->env[is_here]);
//	res = ft_strjoin(ft_strjoin(input, "="), value);
//	res = ft_strjoin(res, value);
//	clear_string(minishell->env[is_here]);
//	ft_strlcpy(minishell->env[is_here], res, ft_strlen(res)+1);
//	ft_printf("vla env : [%s]\n", minishell->env[is_here]);
//	return (0);
//}

/**
 *
 * @param cmd
 * @param minishell
 */

void	command_export(t_cmd *cmd, t_minishell *minishell)
{
	if (!cmd->argv[1])
		ft_printf("Je dois print toutes les variables exportees\n");
	if (find_table_args(minishell->env, cmd->argv[1]) == -1)
		ft_printf("Go function to create new node in env");
	(void)cmd;
	(void)minishell;
}
