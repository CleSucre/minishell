/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/10 09:13:56 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handle the command type
 *
 * @param t_ast **tmp
 * @param t_type *type
 * @return void
 */
static void	handle_command_type(t_ast **tmp, t_type *type)
{
	if (*tmp && ast_get_last(*tmp) && *type == COMMAND)
		*type = TEXT;
	if (*tmp && ast_get_last(*tmp)
		&& (ast_get_last(*tmp)->type == AND_OPERATOR
			|| ast_get_last(*tmp)->type == OR_OPERATOR
			|| ast_get_last(*tmp)->type == SEMICOLON))
		*type = COMMAND;
}

/**
 * @brief Handle the redirect in (<) operator
 *
 * @param t_ast **tmp
 * @param char **args
 * @param int *i
 * @return int 1 if the redirect in was handled, 0 otherwise
 */
static int	handle_redirect_in(t_ast **tmp, char **args, int *i)
{
	if (args[*i + 1] && token_type_primary(args[*i + 1]) == REDIRECT_IN)
	{
		ast_add_last(tmp, create_ast(REDIRECT_IN, args[++(*i)]));
		ast_add_children(ast_get_last(*tmp),
			create_ast(FILE_NAME, args[(*i) - 1]));
		(*i)++;
		return (1);
	}
	return (0);
}

/**
 * @brief Split the text using variables as separator,
 * and add the text and variables to the ast
 *
 * @param t_type type
 * @param t_ast **ast
 * @param char *str
 * @return int 1 if handled, 0 otherwise
 */
static int	handle_text(t_type type, t_ast **ast, char *str)
{
	char	*res;

	res = NULL;
	if (type == TEXT_DOUBLE_QUOTE)
	{
		ast_add_last(ast, create_ast(TEXT_DOUBLE_QUOTE, str));
		res = ft_strtrim(str, "\"");
		if (res)
			ast_add_children(ast_get_last(*ast), create_ast(TEXT, res));
		return (1);
	}
	else if (type == TEXT_SINGLE_QUOTE)
	{
		ast_add_last(ast, create_ast(TEXT_SINGLE_QUOTE, str));
		res = ft_strtrim(str, "'");
		if (res)
			ast_add_children(ast_get_last(*ast), create_ast(TEXT, res));
		return (1);
	}
	else if (type == TEXT)
	{
		ast_add_last(ast, create_ast(TEXT, str));
		return (1);
	}
	return (0);
}

/**
 * @brief Extract the children of a command
 *
 * @param t_ast *ast the ast to add the children to
 * @param char *full_command the command to extract the children from
 * @return void
 */
void	parse_args(t_ast *ast, char **args)
{
	t_ast	*tmp;
	t_type	type;
	int		i;

	tmp = NULL;
	i = 0;
	while (args[i])
	{
		type = token_type_primary(args[i]);
		if (handle_redirect_in(&tmp, args, &i))
			continue ;
		handle_command_type(&tmp, &type);
		if (handle_text(type, &tmp, args[i]))
		{
			i++;
			continue ;
		}
		ast_add_last(&tmp, create_ast(type, args[i++]));
		if (type == REDIRECT_OUT)
			ast_add_children(ast_get_last(tmp),
				create_ast(FILE_NAME, args[i++]));
		if (!ast->children)
			ast->children = tmp;
	}
}
