/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 06:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char **get_args(t_ast *cmd)
{
	char	**args;
	t_ast	*tmp;
	int 	size;
	int 	i;

	size = ast_len(cmd);
	args = ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	tmp = cmd->next;
	while (tmp)
	{
		args[i] = ft_strdup(tmp->value);
		tmp = tmp->next;
		i++;
	}
	args[size] = 0;
	return (args);
}

/**
 * @brief Generate a command structure from an AST node
 *
 * @param t_minishell *minishell
 * @param t_ast *cmd
 * @return t_cmd *
 */
t_cmd	*command_maker(t_minishell *minishell, t_ast *cmd)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->cmd = ft_strdup(cmd->value);
	new_cmd->argv = get_args(cmd);
	new_cmd->argc = ast_len(cmd);
	new_cmd->env = minishell->env;
	new_cmd->path = get_var_value(minishell->env, "PATH");
	new_cmd->fd_in = 0;
	new_cmd->fd_out = 1;
	new_cmd->exit_status = 0;
	return (new_cmd);
}