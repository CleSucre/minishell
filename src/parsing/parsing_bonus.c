/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:27:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/11 19:27:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Process a logical operator token (&& or ||)
 * 			and create an AST operator node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param t_ast_node **root Root of the AST being constructed.
 * @param t_ast_node **last_command Last command processed.
 * @return t_ast_node* The operator node created.
 */
int	process_operator(t_token **tokens, t_ast_node **root,
							t_ast_node **last_command)
{
	t_ast_node_type	op_type;
	t_ast_node		*operator_node;
	t_ast_node		*new_last_command;

	if ((*tokens)->type == TOKEN_AND_OPERATOR)
		op_type = AST_AND;
	else
		op_type = AST_OR;
	operator_node = new_ast_node(op_type, NULL);
	if (*root == NULL)
	{
		operator_node->left = *last_command;
		*root = operator_node;
	}
	else
	{
		operator_node->left = *root;
		*root = operator_node;
	}
	*tokens = (*tokens)->next;
	new_last_command = NULL;
	return (build_ast(tokens, &operator_node->right, &new_last_command));
}

/**
 * @brief Process a subshell (parentheses) and create an AST subshell node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param t_ast_node **root Root of the AST being constructed.
 * @param t_ast_node **last_command Last command processed.
 */
int	process_subshell(t_token **tokens, t_ast_node **root,
					t_ast_node **last_command)
{
	t_ast_node	*subshell_node;

	if ((*tokens)->prev != NULL && (*tokens)->prev->type == TOKEN_COMMAND)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `('\n");
		return (0);
	}
	subshell_node = new_ast_node(AST_SUBSHELL, NULL);
	*tokens = (*tokens)->next;
	if (build_ast(tokens, &subshell_node->left, last_command) != -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error: expected ')'\n");
		return (0);
	}
	while (*tokens != NULL && (*tokens)->type != TOKEN_PARENTHESIS_CLOSE)
		*tokens = (*tokens)->next;
	*tokens = (*tokens)->next;
	if (*root == NULL)
		*root = subshell_node;
	else if (*last_command != NULL)
		subshell_node->right = subshell_node;
	return (1);
}

/**
 * @brief Get the redirection type from the token type.
 *
 * @param t_token_type type Token type.
 * @return t_ast_node_type AST node type.
 */
static t_ast_node_type	get_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_OUT)
		return (AST_REDIR_OUT);
	else if (type == TOKEN_REDIR_OUT_APPEND)
		return (AST_REDIR_OUT_APPEND);
	else if (type == TOKEN_REDIR_IN)
		return (AST_REDIR_IN);
	else
		return (AST_HEREDOC);
}

/**
 * @brief Process a redirection token (>, >>, <, <<)
 * 			and create an AST redirection node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param t_ast_node **root Root of the AST being constructed.
 * @return int 1 if the function succeeded, 0 otherwise.
 */
int	process_redirection(t_token **tokens, t_ast_node **root,
							t_ast_node **last_command, int is_last)
{
	t_ast_node_type	redir_type;
	t_ast_node		*redir_node;
	char			**file_tokens;

	(*last_command)->is_last = is_last;
	redir_type = get_redir_type((*tokens)->type);
	redir_node = new_ast_node(redir_type, NULL);
	redir_node->left = *root;
	*tokens = (*tokens)->next;
	if ((*tokens) == NULL)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error: expected file name\n");
		return (0);
	}
	if ((*tokens)->type == TOKEN_COMMAND)
	{
		file_tokens = extract_command_tokens(tokens);
		redir_node->right = new_ast_node(AST_COMMAND, file_tokens);
	}
	*root = redir_node;
	return (1);
}
