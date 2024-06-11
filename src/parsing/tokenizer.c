/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get the type of token from string
 *
 * @param char *str
 * @return t_type
 */
static t_type	token_type(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(str, "<") == 0)
		return (REDIRECT_IN);
	else if (ft_strcmp(str, ">") == 0)
		return (REDIRECT_OUT);
	else if (ft_strcmp(str, ">>") == 0)
		return (REDIRECT_APPEND);
	else if (ft_strncmp(str, "-", 1) == 0)
		return (FLAG);
	else if (ft_strncmp(str, "$", 1) == 0)
		return (VARIABLE);
	else if (ft_strncmp(str, "\"", 1) == 0)
		return (TEXT_DOUBLE_QUOTE);
	else if (ft_strncmp(str, "\'", 1) == 0)
		return (TEXT_SINGLE_QUOTE);
	else
		return (COMMAND);
}

/**
 * @brief Create a token object
 *
 * @param t_type type of token
 * @param char *value value of token
 * @return
 */
t_token	*create_token(t_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	return (token);
}

/**
 * @brief Tokenize an argument from a command
 *
 * @param char *arg
 * @return t_ast *
 */
t_ast	*tokenize(char *arg)
{
	// If arg contain any token inside a token its self then
	// create a new ast as a child of the current ast (recursion).
	// else create a token object.
	t_ast	*tmp;
	char	**tokens;
	int		i;
	
	tokens = ft_split_quote(arg, "<>|");
	i = 0;
	tmp = ast;
	while (tokens[i])
	{
		if (i % 2 == 0)
		{
			tmp->next = create_ast(token_type(tokens[i]), tokens[i]);
			tmp = tmp->next;
		}
		else
		{
			tmp->next = create_ast(COMMAND, tokens[i]);
			tmp = tmp->next;
		}
		i++;
	}
	return (ast);
}

/**
 * @brief Extract the children of a command
 *
 * @param t_ast *ast the ast to add the children to
 * @param char *full_command the command to extract the children from
 * @return t_ast *
 */
void	extract_args(t_ast	*ast, char *full_command)
{
	t_ast	*tmp;
	char	**args;
	int		i;

	args = ft_split_quote(full_command, WHITESPACES);
	if (!args)
		return ;
	i = 0;
	tmp = create_ast(COMMAND, args[i]);
	ast->children = tmp;
	while (args[++i])
	{
		tmp->next = tokenize(tmp, args[i]);
		tmp = tmp->next;
	}
	free(args);
}
