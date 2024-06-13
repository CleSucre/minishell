/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Extract variables from a string
 *
 * @param char *str
 * @return t_ast *
 */
static t_ast	*extract_variables(char *str)
{
	char	*trimmed;
	t_ast	*tmp;

	trimmed = ft_strtrim(str, "\"");
	tmp = create_ast(TEXT, trimmed);
	return (tmp);
}

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

static int	handle_double_quote(int type, t_ast **tmp, char **args, int *i)
{
	if (type == TEXT_DOUBLE_QUOTE)
	{
		ast_add_last(tmp, create_ast(TEXT_DOUBLE_QUOTE, args[*i]));
		ast_add_children(ast_get_last(*tmp), extract_variables(args[(*i)++]));
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
void	extract_args(t_ast *ast, char **args)
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
		if (handle_double_quote(type, &tmp, args, &i))
			continue ;
		ast_add_last(&tmp, create_ast(type, args[i++]));
		if (type == REDIRECT_OUT)
			ast_add_children(ast_get_last(tmp),
				create_ast(FILE_NAME, args[i++]));
		if (!ast->children)
			ast->children = tmp;
	}
}
