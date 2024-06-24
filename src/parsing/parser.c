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
 * @brief Extract full custom from input by splitting it with '|'
 * 			It also handle single and double quotes
 *
 * @param char *input
 * @return t_ast *
 */
static t_ast	*extract_full_commands(char *input)
{
	char	**commands;
	void	*tmp;
	t_ast	*ast;
	int		i;

	commands = ft_split_quote(input, "|", "\"'");
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

static char **add_space_between_words(char **args)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	while (args[i])
	{
		tmp = ft_strjoin(args[i], " ");
		tmp2 = ft_strjoin(tmp, args[i + 1]);
		free(args[i]);
		free(args[i + 1]);
		args[i] = tmp2;
		i++;
	}
}

/**
 * @brief Extract arguments from a full command by splitting it with WHITESPACES
 *
 * @param t_ast *ast ast containing all full custom
 * @return void *
 */
static void	*parse_ast(t_minishell *minishell, t_ast *ast)
{
	t_ast	*tmp;
	char	**args;

	if (!ast)
		return (NULL);
	tmp = ast;
	while (tmp)
	{
		if (tmp->type == FULL_COMMAND)
		{
			if (ft_strchr(tmp->value, '"') && ft_strchr(tmp->value, '\''))
				args = ft_split_quote(tmp->value, WHITESPACES, "\"'");
			else if (ft_strchr(tmp->value, '"'))
				args = ft_split_quote(tmp->value, "\"", "'");
			else if (ft_strchr(tmp->value, '\''))
				args = ft_split_quote(tmp->value, "'", "\"");
			else
			{
				args = ft_split(tmp->value, WHITESPACES);
				//set space between each word
				add_space_between_words(args);
			}
			if (!args)
				return (NULL);
			extract_args(minishell, tmp, args);
			free(args);
		}
		tmp = tmp->next;
	}
	return (ast);
}

/**
 * @brief Parse the input and create ast
 *
 * @param char *input string to parse
 * @return t_ast *
 */
t_ast	*parse_input(t_minishell *minishell, char *input)
{
	t_ast	*ast;

	(void)minishell;
	if (!input)
		return (NULL);
	ast = extract_full_commands(input);
	if (!ast)
		return (NULL);
	parse_ast(minishell, ast);
	debug_ast(ast);
	return (ast);
}
