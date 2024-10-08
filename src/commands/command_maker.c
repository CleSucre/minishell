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

void	destroy_cmd(t_cmd *cmd)
{
	if (cmd->name)
		free(cmd->name);
	if (cmd->path)
		free(cmd->path);
	free(cmd);
}

/**
 * @brief Generate a command structure from an AST node
 *
 * @param t_ast_node *ast: AST node representing the command
 * @param char **envp: Environment variables
 */
t_cmd	*create_cmd(t_ast_node *ast, char **envp, int in_out[3])
{
	t_cmd	*cmd;
	char	*path;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->name = ft_strdup(ast->value[0]);
	cmd->args = ast->value;
	cmd->argc = (int)ft_tablen((const char **)ast->value);
	cmd->input_fd = in_out[0];
	cmd->output_fd = in_out[1];
	cmd->to_close = in_out[2];
	cmd->env = envp;
	cmd->path = NULL;
	cmd->exit_code = 0;
	path = get_path(ast->value[0], envp);
	if (!path)
		path = ft_strdup(ast->value[0]);
	cmd->path = path;
	return (cmd);
}
