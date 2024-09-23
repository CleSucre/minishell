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
t_cmd	*create_cmd(t_ast_node *ast, char **envp, int input_fd, int output_fd)
{
	t_cmd	*cmd;
	char	*path;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->name = strdup(ast->value[0]);
	cmd->args = ast->value;
	cmd->argc = (int)ft_tablen((const char **)ast->value);
	cmd->input_fd = input_fd;
	cmd->output_fd = output_fd;
	cmd->env = envp;
	cmd->path = NULL;
	cmd->exit_code = 0;
	path = get_path(ast->value[0], envp);
	if (!path)
		path = ft_strdup(ast->value[0]);
	cmd->path = path;
	return (cmd);
}
