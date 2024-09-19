/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_maker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 09:39:52 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Allocate memory for the argument array
 *
 * @param t_ast *cmd
 * @return char**
 */
 /*
static char	**allocate_args(t_ast_node *cmd)
{
	char	**args;

	args = ft_calloc(ast_len(cmd) + 2, sizeof(char *));
	if (!args)
		return (NULL);
	return (args);
}
  */

/**
 * @brief Handle different types of AST nodes for argument extraction
 *
 * @param minishell
 * @param tmp
 * @return char*
 */
 /*
static char	*process_node(t_minishell *minishell, t_ast_node *tmp)
{
	char	*arg;

	arg = NULL;
	if (tmp->type == TOKEN_DQUOTE)
	{
		if (tmp->next->type == TOKEN_COMMAND)
			arg = ft_strdup(tmp->next->value);
	}
	else if (tmp->type == TOKEN_DQUOTE)
	{
		if (tmp->next->type == TOKEN_COMMAND)
			arg = replace_variables(minishell->env, tmp->next->value);
	}
	else if (tmp->type == TOKEN_VARIABLE)
		arg = get_var_value(minishell->env, tmp->value + 1);
	else if (tmp->type == TOKEN_COMMAND)
		arg = replace_variables(minishell->env, tmp->value);
	else
		arg = ft_strdup(tmp->value);
	return (arg);
}
  */

/**
 * @brief Extract the arguments from the AST node
 *
 * TODO: handle the case where there is commands in a next node
 *
 * @param t_minishell *minishell
 * @param t_ast *cmd
 * @return char**
 */
 /*
static char	**get_argv(t_minishell *minishell, t_ast_node *cmd)
{
	char	**args;
	t_ast_node	*tmp;
	int		i;

	args = allocate_args(cmd);
	if (!args)
		return (NULL);
	i = 0;
	args[i++] = ft_strdup(cmd->value);
	tmp = cmd->next;
	while (tmp)
	{
		args[i] = process_node(minishell, tmp);
		tmp = tmp->next;
		i++;
	}
	args[ast_len(cmd)] = NULL;
	return (args);
}
  */

/**
 * @brief Generate a command structure from an AST node
 *
 * @param t_minishell *minishell
 * @param t_ast *cmd
 * @param int input
 * @param int output
 * @param int to_close
 * @return t_cmd *
 */
 /*
t_cmd	*load_command(t_minishell *minishell, t_ast_node *ast_cmd,
		const int in_out[2], int to_close)
{
	t_cmd	*new_cmd;
	char	*path;

	new_cmd = ft_calloc(1, sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->cmd_name = ft_strdup(ast_cmd->value);
	path = get_path(ast_cmd->value, minishell->env);
	if (!path)
		path = ft_strdup(ast_cmd->value);
	new_cmd->path = path;
	//new_cmd->argv = get_argv(minishell, ast_cmd);
	//new_cmd->argc = (int)ast_len(ast_cmd);
	new_cmd->env = minishell->env;
	new_cmd->input = in_out[0];
	new_cmd->output = in_out[1];
	new_cmd->to_close = to_close;
	new_cmd->exit_status = 0;
	new_cmd->pid = -1;
	return (new_cmd);
}
*/