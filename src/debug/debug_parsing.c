/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_token_type_secondary(int type)
{
	if (type == ARGUMENT)
		return ("ARGUMENT");
	else if (type == FULL_COMMAND)
		return ("FULL_COMMAND");
	else if (type == COMMAND)
		return ("COMMAND");
	else if (type == FILE_NAME)
		return ("FILE_NAME");
	else if (type == AND_OPERATOR)
		return ("AND_OPERATOR");
	else if (type == OR_OPERATOR)
		return ("OR_OPERATOR");
	else if (type == SEMICOLON)
		return ("SEMICOLON");
	return ("UNKNOWN");
}

/**
 * @brief Get token type as string
 *
 * @param int type token type
 * @return char * token type as string
 */
static char	*get_token_type_primary(int type)
{
	if (type == PIPE)
		return ("PIPE");
	else if (type == REDIRECT_IN)
		return ("REDIRECT_IN");
	else if (type == REDIRECT_OUT)
		return ("REDIRECT_OUT");
	else if (type == REDIRECT_APPEND)
		return ("REDIRECT_APPEND");
	else if (type == FLAG)
		return ("FLAG");
	else if (type == VARIABLE)
		return ("VARIABLE");
	else if (type == TEXT)
		return ("TEXT");
	else if (type == TEXT_DOUBLE_QUOTE)
		return ("TEXT_DOUBLE_QUOTE");
	else if (type == TEXT_SINGLE_QUOTE)
		return ("TEXT_SINGLE_QUOTE");
	return (get_token_type_secondary(type));
}

/**
 * @brief Print tokens list
 *
 * @param t_token *tokens list of tokens
 * @return void
 */
void	debug_tokens(t_token *tokens)
{
	int		i;

	if (!DEBUG)
		return ;
	i = 0;
	while (tokens)
	{
		ft_printf("\n%s[DEBUG] ====== tokens [%d] ======%s\n", BLUE, i, RESET);
		ft_printf("token: %s%s%s\n", YELLOW, tokens->value, RESET);
		ft_printf("type str: %s%s%s (id: %d)\n",
			BOLDWHITE, get_token_type_primary(tokens->type),
			RESET, tokens->type);
		tokens = tokens->next;
		i++;
	}
}

void	debug_children(t_ast *ast, int level)
{
	char	*space;

	if (!DEBUG)
		return ;
	while (ast)
	{
		space = ft_calloc((level * 8) + 1, sizeof(char));
		ft_memset(space, ' ', (level * 8));
		ft_printf("\n%s%s[DEBUG] ====== CHILDREN ======%s\n",
			space, BLUE, RESET);
		ft_printf("%stype str: %s%s%s (id: %d)\n",
			space, BOLDWHITE, get_token_type_primary(ast->type),
			RESET, ast->type);
		ft_printf("%svalue: %s%s%s\n", space, YELLOW, ast->value, RESET);
		free(space);
		if (ast->children)
			debug_children(ast->children, level + 1);
		ast = ast->next;
	}
}

void	debug_ast(t_ast *ast)
{
	if (!DEBUG)
		return ;
	while (ast)
	{
		ft_printf("\n%s[DEBUG] ====== AST ======%s\n", BLUE, RESET);
		ft_printf("type str: %s%s%s (id: %d)\n",
			BOLDWHITE, get_token_type_primary(ast->type), RESET, ast->type);
		ft_printf("value: %s%s%s\n", YELLOW, ast->value, RESET);
		if (ast->children)
			debug_children(ast->children, 1);
		ast = ast->next;
	}
}
