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
static char	*get_token_type(t_token_type type)
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
static char	*get_ast_node_type_secondary(t_ast_node_type type)
{
	if (type == AST_REDIR_OUT)
		return ("AST_REDIR_OUT");
	else if (type == AST_REDIR_OUT_APPEND)
		return ("AST_REDIR_OUT_APPEND");
	else if (type == AST_REDIR_IN)
		return ("AST_REDIR_IN");
	else if (type == AST_HEREDOC)
		return ("AST_HEREDOC");
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
				  BOLDWHITE, get_token_type(tokens->type),
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
	int		i;

	space = ft_calloc((level * 4) + 1, sizeof(char));
	if (!space)
		return ;
	ft_memset(space, ' ', level * 4);

	ft_printf("\n%s%s[DEBUG] ====== NODE ======%s\n", space, BLUE, RESET);
	ft_printf("%stype str: %s%s%s (id: %d)\n", space, BOLDWHITE,
			  get_ast_node_type_primary(ast->type), RESET, ast->type);

	// Print the command and its arguments if it's a command node
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

/**
 * @brief Helper function to print child AST nodes
 *
 * @param child Child AST node
 * @param level Indentation level
 * @param label Label to print before the child node
 */
static void	debug_ast_child(t_ast_node *child, int level, const char *label)
{
	if (child)
	{
		ft_printf("%s\n", label);
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

	// Afficher les informations du nœud courant
	debug_ast_node(ast, level);

	// Afficher les enfants (gauche et droite)
	debug_ast_child(ast->left, level, "Left next:");
	debug_ast_child(ast->right, level, "Right next:");
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

	ft_printf("\n%s[DEBUG] ====== AST NODE ======%s\n", BLUE, RESET);
	ft_printf("type: %s%s%s (id: %d)\n", BOLDWHITE, get_ast_node_type_primary(ast->type), RESET, ast->type);

	// Imprimer les arguments de la commande (si c'est un nœud de commande)
	if (ast->type == AST_COMMAND)
	{
		ft_printf("command and args: ");
		for (int i = 0; ast->value && ast->value[i]; i++)
			ft_printf("%s%s ", YELLOW, ast->value[i]);
		ft_printf("%s\n", RESET);
	}

	// Imprimer les enfants
	debug_ast_child(ast->left, 1, "Left next:");
	debug_ast_child(ast->right, 1, "Right next:");
}
