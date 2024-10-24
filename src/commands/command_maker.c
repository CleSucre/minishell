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

char	*parse_quotes(const char *input)
{
	char	*result;
	int		i;
	int		j;
	char	quote_type;

	result = (char *)malloc(ft_strlen(input) + 1);
	i = -1;
	j = 0;
	quote_type = 0;
	while (input[++i] != '\0')
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			if (quote_type == 0)
				quote_type = input[i];
			else if (quote_type == input[i])
				quote_type = 0;
			else
				result[j++] = input[i];
		}
		else
			result[j++] = input[i];
	}
	result[j] = '\0';
	return (result);
}

void	remove_quotes(char **table)
{
	int		i;
	char	*tmp;

	i = 0;
	while (table[i])
	{
		tmp = table[i];
		table[i] = parse_quotes(tmp);
		free(tmp);
		i++;
	}
}

/**
 * @brief Replace variables, support wildcard expansion
 *
 * @param t_minishell *minishell Minishell structure
 * @param char **tab Tab of strings
 */
static void	parse_cmd_args(t_cmd *cmd, t_ast_node *ast, t_minishell *minishell)
{
	cmd->argc = (int)ft_tablen((const char **)ast->value);
	cmd->args = ft_tabdup((const char **)ast->value);
	if (!cmd->args)
		return ;
	if (ast->type != AST_HEREDOC)
		replace_variables_in_tab(minishell, cmd->args);
	replace_variables_in_tab(minishell, cmd->args);
	remove_quotes(cmd->args);
	if (cmd->args[0] == NULL)
		return ;
	expand_wildcards(&cmd->args);
	if (!cmd->args[0])
	{
		ft_tabfree(cmd->args);
		cmd->args = ft_tabnew(1);
		return ;
	}
	cmd->name = ft_strdup(cmd->args[0]);
	cmd->argc = (int)ft_tablen((const char **)cmd->args);
}

/**
 * @brief Free a command structure
 *
 * @param t_cmd *cmd Command structure to free
 */
void	destroy_cmd(t_cmd *cmd)
{
	if (cmd->name)
	{
		free(cmd->name);
		cmd->name = NULL;
	}
	if (cmd->path)
	{
		free(cmd->path);
		cmd->path = NULL;
	}
	ft_tabfree(cmd->args);
	free(cmd);
	cmd = NULL;
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
	cmd->name = NULL;
	cmd->path = NULL;
	cmd->args = NULL;
	parse_cmd_args(cmd, ast, minishell);
	if (cmd->args[0] == NULL)
	{
		destroy_cmd(cmd);
		return (NULL);
	}
	cmd->input_fd = in_out[0];
	cmd->output_fd = in_out[1];
	cmd->to_close = in_out[2];
	cmd->env = minishell->env;
	path = get_path(cmd->args[0], minishell->env);
	if (!path)
		path = ft_strdup(cmd->args[0]);
	cmd->path = path;
	cmd->exit_signal = 0;
	return (cmd);
}
