/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/10 09:14:52 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get token type as string (primary)
 *
 * @param type Token type ID
 * @return char* Token type as string
 */
char	*get_token_type(t_token_type type)
{
	if (type == TOKEN_COMMAND)
		return ("TOKEN_COMMAND");
	else if (type == TOKEN_ARGUMENT)
		return ("TOKEN_ARGUMENT");
	else if (type == TOKEN_AND_OPERATOR)
		return ("TOKEN_AND_OPERATOR");
	else if (type == TOKEN_OR_OPERATOR)
		return ("TOKEN_OR_OPERATOR");
	else if (type == TOKEN_PIPE)
		return ("TOKEN_PIPE");
	else if (type == TOKEN_REDIR_OUT)
		return ("TOKEN_REDIR_OUT");
	else if (type == TOKEN_REDIR_OUT_APPEND)
		return ("TOKEN_REDIR_OUT_APPEND");
	else if (type == TOKEN_REDIR_IN)
		return ("TOKEN_REDIR_IN");
	else if (type == TOKEN_HEREDOC)
		return ("TOKEN_HEREDOC");
	else if (type == TOKEN_PARENTHESIS_OPEN)
		return ("TOKEN_PARENTHESIS_OPEN");
	else if (type == TOKEN_PARENTHESIS_CLOSE)
		return ("TOKEN_PARENTHESIS_CLOSE");
	return ("UNKNOWN");
}

/**
 * @brief Get AST node type as string (secondary)
 *
 * @param type AST node type ID
 * @return char* AST node type as string
 */
char	*get_ast_node_type_secondary(t_ast_node_type type)
{
	if (type == AST_REDIR_OUT)
		return ("AST_REDIR_OUT");
	else if (type == AST_REDIR_OUT_APPEND)
		return ("AST_REDIR_OUT_APPEND");
	else if (type == AST_REDIR_IN)
		return ("AST_REDIR_IN");
	else if (type == AST_HEREDOC)
		return ("AST_HEREDOC");
	return ("UNKNOWN");
}

/**
 * @brief Get AST node type as string (primary)
 *
 * @param type AST node type ID
 * @return char* AST node type as string
 */
char	*get_ast_node_type_primary(t_ast_node_type type)
{
	if (type == AST_COMMAND)
		return ("AST_COMMAND");
	else if (type == AST_PIPE)
		return ("AST_PIPE");
	else if (type == AST_AND)
		return ("AST_AND");
	else if (type == AST_OR)
		return ("AST_OR");
	else if (type == AST_SUBSHELL)
		return ("AST_SUBSHELL");
	return (get_ast_node_type_secondary(type));
}

/**
 * @brief Helper function to print a single AST node
 *
 * @param ast AST node
 * @param level Indentation level
 */
void	debug_ast_node(t_ast_node *ast, int level)
{
	char	*space;
	int		i;

	space = ft_calloc((level * 4) + 1, sizeof(char));
	if (!space)
		return ;
	ft_memset(space, ' ', level * 4);
	ft_printf("\n%s%s[DEBUG] ====== NODE ======%s\n", space, BLUE, RESET);
	ft_printf("%stype str: %s%s%s (id: %d | is_last: %d)\n", space,
		BOLDWHITE, get_ast_node_type_primary(ast->type), RESET,
		ast->type, ast->is_last);
	if (ast->value && ast->type == AST_COMMAND)
	{
		ft_printf("%scommand and args:\n", space);
		i = 0;
		while (ast->value[i])
		{
			ft_printf("%s- %s%s%s\n", space, CYAN, ast->value[i], RESET);
			i++;
		}
	}
	free(space);
}
