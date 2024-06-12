/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Extract full commands from input by splitting it with '|'
 * 			It also handle single and double quotes
 *
 * @param minishell
 * @param input
 * @return
 */
static t_ast	*extract_full_commands(t_minishell *minishell, char *input)
{
	char	**commands;
	void	*tmp;
	t_ast	*ast;
	int		i;

	(void)minishell;
	commands = ft_split_quote(input, "|");
	if (!commands)
		return (NULL);
	i = 0;
	ast = create_ast(FULL_COMMAND, commands[i]);
	while (commands[++i])
	{
		tmp = ft_strtrim(commands[i], " ");
		free(commands[i]);
		commands[i] = tmp;
		tmp = create_ast(FULL_COMMAND, commands[i]);
		if (!tmp)
		{
			free_ast(ast);
			return (NULL);
		}
		ast_add_last(&ast, tmp);
	}
	free(commands);
	return (ast);
}

static void	*parse_ast(t_ast *ast)
{
	t_ast	*tmp;

	if (!ast)
		return (NULL);
	tmp = ast;
	while (tmp)
	{
		if (tmp->type == FULL_COMMAND)
			extract_args(tmp, tmp->value);
		tmp = tmp->next;
	}
	return (ast);
}

/**
 * @brief Parse the input and create ast
 *
 * @param char *input input to parse
 * @return t_ast *
 */
t_ast	*parse_input(t_minishell *minishell, char *input)
{
	t_ast	*ast;

	(void)minishell;
	if (!input)
		return (NULL);
	ast = extract_full_commands(minishell, input);
	if (!ast)
		return (NULL);
	parse_ast(ast);
	debug_ast(ast);
	return (ast);
}
