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

static void	parse_cmd_args(t_cmd *cmd, t_ast_node *ast, t_minishell *minishell)
{
	int 	is_var;
	char 	**tmp;

	is_var = ft_strncmp(ast->value[0], "$", 1) == 0;
	replace_variables_in_tab(minishell, ast->value);
	if (is_var)
	{
		tmp = ft_split(ast->value[0], WHITESPACES);
		cmd->name = ft_strdup(tmp[0]);
		ft_tabdel(ast->value, 0);
		ast->value = ft_tabjoin(tmp, ast->value);
	}
	else
		cmd->name = ft_strdup(ast->value[0]);
	cmd->args = ast->value;
	cmd->argc = (int)ft_tablen((const char **)ast->value);
}

/**
 * @brief Free a command structure
 *
 * @param t_cmd *cmd Command structure to free
 */
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
 * @param char **envp Environment variables
 * @param int in_out[3] File descriptors for input, output and to close
 * @return t_cmd* Command structure generated
 */
t_cmd	*create_cmd(t_ast_node *ast, t_minishell *minishell,
					const int in_out[3])
{
	t_cmd	*cmd;
	char	*path;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	parse_cmd_args(cmd, ast, minishell);
	cmd->input_fd = in_out[0];
	cmd->output_fd = in_out[1];
	cmd->to_close = in_out[2];
	cmd->env = minishell->env;
	cmd->path = NULL;
	path = get_path(ast->value[0], minishell->env);
	if (!path)
		path = ft_strdup(ast->value[0]);
	cmd->path = path;
	cmd->exit_signal = 0;
	return (cmd);
}
