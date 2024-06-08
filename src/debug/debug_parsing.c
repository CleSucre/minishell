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

/**
 * @brief Get token type as string
 *
 * @param int type token type
 * @return char * token type as string
 */
static char	*get_token_type(int type)
{
	if (type == PIPE)
		return ("PIPE");
	else if (type == REDIRECT_IN)
		return ("REDIRECT_IN");
	else if (type == REDIRECT_OUT)
		return ("REDIRECT_OUT");
	else if (type == REDIRECT_APPEND)
		return ("REDIRECT_APPEND");
	else if (type == FULL_COMMAND)
		return ("FULL_COMMAND");
	else if (type == COMMAND)
		return ("COMMAND");
	else if (type == ARGUMENT)
		return ("ARGUMENT");
	else if (type == QUOTE)
		return ("QUOTE");
	else if (type == VARIABLE)
		return ("VARIABLE");
	else if (type == FLAG)
		return ("FLAG");
	else
		return ("UNKNOWN");
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
			BOLDWHITE, get_token_type(tokens->type), RESET, tokens->type);
		tokens = tokens->next;
		i++;
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
				  BOLDWHITE, get_token_type(ast->type), RESET, ast->type);
		ft_printf("value: %s%s%s\n", YELLOW, ast->value, RESET);
		debug_tokens(ast->tokens);
		ast = ast->next;
	}
}