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

static char	**extract_full_commands(t_minishell *minishell, char *input)
{
	char	**commands;
	char	*tmp;
	int		i;

	(void)minishell;
	commands = ft_split(input, "|");
	if (!commands)
		return (NULL);
	i = 0;
	while (commands[i])
	{
		tmp = ft_strtrim(commands[i], " ");
		free(commands[i]);
		commands[i] = tmp;
		i++;
	}
	return (commands);
}

static t_ast	*parse_command(char *command)
{
	t_ast	*ast;

	if (!command)
		return (NULL);
	ast = create_ast(FULL_COMMAND, command);
	if (!ast)
		return (NULL);
	ast->tokens = tokenize(command);
	return (ast);
}

void	ast_add_back(t_ast *head, t_ast *ast);

/**
 * @brief Parse the input and create ast
 *
 * @param char *input input to parse
 * @return t_ast *
 */
t_ast	*parse_input(t_minishell *minishell, char *input)
{
	t_ast	*ast;
	t_ast	*tmp;
	char	**commands;
	int		i;

	(void)minishell;
	if (!input)
		return (NULL);
	commands = extract_full_commands(minishell, input);
	if (!commands)
		return (NULL);
	i = 0;
	ast = parse_command(commands[i]);
	if (!ast)
		return (NULL);
	i++;
	while (commands[i])
	{
		tmp = parse_command(commands[i]);
		if (!tmp)
		{
			free_ast(ast);
			return (NULL);
		}
		ast_add_back(ast, tmp);
		i++;
	}
	free(commands);
	//debug_ast(ast);
	return (ast);
}
