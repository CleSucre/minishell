/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                           :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 06:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Print the history of commands, support for -c and -s flags
 *
 * TODO: avoid saving history -s command in history
 *
 * @param t_cmd *cmd
 * @param t_minishell *minishell
 * @return void
 */
void	command_history(t_cmd *cmd, t_minishell *minishell)
{
	char	*tmp;

	if (cmd->argc == 1)
		history_print(minishell, cmd->output);
	else if (contain_flag(cmd->argv, 'c'))
	{
		terminal_print("History cleared\n", 1, cmd->output);
		history_reset(minishell);
	}
	if (contain_flag(cmd->argv, 's') && cmd->argc >= 3)
	{
		tmp = ft_strjoin_list((const char **)(cmd->argv + 2), " ");
		if (!tmp)
			return ;
		history_add(minishell, tmp, 1);
		free(tmp);
		terminal_print("", 1, cmd->output);
	}
}
