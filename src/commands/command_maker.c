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

static char **get_argv(t_minishell *minishell, t_ast *cmd)
{
	char			**args;
	t_ast			*tmp;
	unsigned int	size;
	int 			i;

	(void)minishell;
	size = ast_len(cmd);
	args = ft_calloc(size + 2, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	args[i++] = ft_strdup(cmd->value);
	tmp = cmd->next;
	while (tmp)
	{
		if (tmp->type == TEXT_SINGLE_QUOTE)
		{
			if (tmp->children->type == TEXT)
				args[i] = ft_strdup(tmp->children->value);
		}
		else if (tmp->type == TEXT_DOUBLE_QUOTE)
		{
			if (tmp->children->type == TEXT)
				args[i] = replace_variables(minishell->env, tmp->children->value);
			//TODO: handle the case where the children is a command or else
		}
		else if (tmp->type == VARIABLE)
			args[i] = get_var_value(minishell->env, tmp->value + 1);
		else if (tmp->type == TEXT)
			args[i] = replace_variables(minishell->env, tmp->value);
		else
			args[i] = ft_strdup(tmp->value);
		tmp = tmp->next;
		i++;
	}
	args[size] = NULL;
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
	char	*path;

	new_cmd = ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	//find the command in the PATH
	new_cmd->cmd_name = ft_strdup(cmd->value);
	path = get_path(cmd->value, minishell->env);
	if (!path)
		path = ft_strdup(cmd->value);
	new_cmd->cmd_exec = path;
	new_cmd->argv = get_argv(minishell, cmd);
	new_cmd->argc = (int)ast_len(cmd);
	new_cmd->env = minishell->env;
	new_cmd->path = get_var_value(minishell->env, "PATH");
	new_cmd->fd_in = 0;
	new_cmd->fd_out = 1;
	new_cmd->exit_status = 0;
	return (new_cmd);
}