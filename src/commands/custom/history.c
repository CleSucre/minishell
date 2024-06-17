/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 06:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int command_have_flag(char **argv, char flag)
{
	int i;
	int j;

	i = 0;
	while (argv[i])
	{
		if (argv[i][0] == '-')
		{
			j = 1;
			while (argv[i][j])
			{
				if (argv[i][j] == flag)
					return (1);
				j++;
			}
		}
		i++;
	}
	return (0);
}

/**
 * @brief Print the history of commands, support for -c and -s flags
 *
 * @param t_cmd *cmd
 * @param t_minishell *minishell
 * @return void
 */
void	command_history(t_cmd *cmd, t_minishell *minishell)
{
	char *tmp;

	if (cmd->argc == 1)
	{
		history_print(minishell);
	}
	if (command_have_flag(cmd->argv, 'c'))
	{
		ft_printf("Clearing history\n");
		history_reset(minishell);
	}
	if (command_have_flag(cmd->argv, 's') && cmd->argc >= 3)
	{
		//join every args after -s flag
		tmp = ft_strjoin_list((const char **)cmd->argv + 2, " ");
		history_add(minishell, tmp, 1);
		free(tmp);
	}
}