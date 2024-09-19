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
 * @brief Get token type as string (secondary)
 *
 * @param type Token type ID
 * @return char* Token type as string
 */
static char	*get_token_type_secondary(t_token_type type)
{
	if (type == TOKEN_REDIR_OUT)
		return ("TOKEN_REDIR_OUT");
	else if (type == TOKEN_REDIR_OUT_APPEND)
		return ("TOKEN_REDIR_OUT_APPEND");
	else if (type == TOKEN_HEREDOC)
		return ("TOKEN_HEREDOC");
	else if (type == TOKEN_ASSIGNMENT)
		return ("TOKEN_ASSIGNMENT");
	else if (type == TOKEN_VARIABLE)
		return ("TOKEN_VARIABLE");
	else if (type == TOKEN_ASTERISK)
		return ("TOKEN_ASTERISK");
	else if (type == TOKEN_EOF)
		return ("TOKEN_EOF");
	return ("UNKNOWN");
}

/**
 * @brief Get token type as string (primary)
 *
 * @param type Token type ID
 * @return char* Token type as string
 */
static char	*get_token_type_primary(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("TOKEN_WORD");
	else if (type == TOKEN_AND)
		return ("TOKEN_AND");
	else if (type == TOKEN_OR)
		return ("TOKEN_OR");
	else if (type == TOKEN_PIPE)
		return ("TOKEN_PIPE");
	else if (type == TOKEN_LPAREN)
		return ("TOKEN_LPAREN");
	else if (type == TOKEN_RPAREN)
		return ("TOKEN_RPAREN");
	else if (type == TOKEN_DQUOTE)
		return ("TOKEN_DQUOTE");
	else if (type == TOKEN_SQUOTE)
		return ("TOKEN_SQUOTE");
	else if (type == TOKEN_REDIR_IN)
		return ("TOKEN_REDIR_IN");
	return (get_token_type_secondary(type));
}

/**
 * @brief Get AST node type as string (secondary)
 *
 * @param type AST node type ID
 * @return char* AST node type as string
 */
static char	*get_ast_node_type_secondary(t_ast_node_type type)
{
	if (type == AST_REDIRECTION)
		return ("AST_REDIRECTION");
	else if (type == AST_ASSIGNMENT)
		return ("AST_ASSIGNMENT");
	else if (type == AST_VARIABLE)
		return ("AST_VARIABLE");
	return ("UNKNOWN");
}

/**
 * @brief Get AST node type as string (primary)
 *
 * @param type AST node type ID
 * @return char* AST node type as string
 */
static char	*get_ast_node_type_primary(t_ast_node_type type)
{
	if (type == AST_COMMAND)
		return ("AST_COMMAND");
	else if (type == AST_PIPE)
		return ("AST_PIPE");
	else if (type == AST_AND)
		return ("AST_AND");
	else if (type == AST_OR)
		return ("AST_OR");
	else if (type == AST_SEQUENCE)
		return ("AST_SEQUENCE");
	else if (type == AST_SUBSHELL)
		return ("AST_SUBSHELL");
	return (get_ast_node_type_secondary(type));
}

/**
 * @brief Print tokens list for debugging purposes
 *
 * @param tokens List of tokens
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

/**
 * @brief Helper function to print a single AST node
 *
 * @param ast AST node
 * @param level Indentation level
 */
static void	debug_ast_node(t_ast_node *ast, int level)
{
	char	*space;

	space = ft_calloc((level * 4) + 1, sizeof(char));
	if (!space)
		return ;
	ft_memset(space, ' ', level * 4);
	ft_printf("\n%s%s[DEBUG] ====== NODE ======%s\n", space, BLUE, RESET);
	ft_printf("%stype str: %s%s%s (id: %d)\n", space, BOLDWHITE,
			  get_ast_node_type_primary(ast->type), RESET, ast->type);
	ft_printf("%svalue: %s|%s%s%s|%s\n", space, CYAN, YELLOW,
			  ast->value ? ast->value : "NULL", CYAN, RESET);
	free(space);
}

/**
 * @brief Helper function to print next nodes
 *
 * @param child Child AST node
 * @param level Indentation level
 * @param label Label to print before the next nodes
 */
static void	debug_ast_child(t_ast_node *child, int level, const char *label)
{
	if (child)
	{
		ft_printf("%s", label);
		debug_children(child, level + 1);
	}
}

/**
 * @brief Print children of an AST node for debugging purposes
 *
 * @param ast AST node
 * @param level Level of the AST node (used for indentation)
 */
void	debug_children(t_ast_node *ast, int level)
{
	if (!DEBUG || !ast)
		return ;
	debug_ast_node(ast, level);
	debug_ast_child(ast->args, level, "Arguments:");
	debug_ast_child(ast->redirs, level, "Redirections:");
	debug_ast_child(ast->left, level, "Left next:");
	debug_ast_child(ast->right, level, "Right next:");
	if (ast->next)
		debug_children(ast->next, level);
}

/**
 * @brief Print AST for debugging purposes
 *
 * @param ast AST node
 */
void	debug_ast(t_ast_node *ast)
{
	if (!DEBUG)
		return ;
	while (ast)
	{
		ft_printf("\n%s[DEBUG] ====== AST NODE ======%s\n", BLUE, RESET);
		ft_printf("type str: %s%s%s (id: %d)\n",
				  BOLDWHITE, get_ast_node_type_primary(ast->type),
				  RESET, ast->type);
		ft_printf("value: %s|%s%s%s|%s\n", CYAN, YELLOW,
				  ast->value ? ast->value : "NULL", CYAN, RESET);
		debug_ast_child(ast->args, 1, "Arguments:");
		debug_ast_child(ast->redirs, 1, "Redirections:");
		debug_ast_child(ast->left, 1, "Left next:");
		debug_ast_child(ast->right, 1, "Right next:");
		ast = ast->next;
	}
}
