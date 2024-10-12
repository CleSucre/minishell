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
t_ast_node	*process_operator(t_token **tokens, t_ast_node **root,
							t_ast_node **last_command)
{
	t_ast_node_type	op_type;
	t_ast_node		*operator_node;

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
	operator_node->right = build_ast(tokens);
	return (*root);
}

/**
 * @brief Process a subshell (parentheses) and create an AST subshell node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param t_ast_node **root Root of the AST being constructed.
 * @param t_ast_node **last_command Last command processed.
 */
void	process_subshell(t_token **tokens, t_ast_node **root,
					t_ast_node **last_command)
{
	t_ast_node	*subshell_node;

	subshell_node = new_ast_node(AST_SUBSHELL, NULL);
	*tokens = (*tokens)->next;
	subshell_node->left = build_ast(tokens);
	while (*tokens != NULL && (*tokens)->type != TOKEN_PARENTHESIS_CLOSE)
		*tokens = (*tokens)->next;
	if (*tokens != NULL && (*tokens)->type == TOKEN_PARENTHESIS_CLOSE)
	{
		*tokens = (*tokens)->next;
	}
	if (*root == NULL)
		*root = subshell_node;
	else if (*last_command != NULL)
		subshell_node->right = subshell_node;
}

/**
 * @brief Process a redirection token (>, >>, <, <<)
 * 			and create an AST redirection node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param t_ast_node **root Root of the AST being constructed.
 * @return t_ast_node* The redirection node created.
 */
t_ast_node	*process_redirection(t_token **tokens, t_ast_node **root, t_ast_node **last_command, int is_last)
{
	t_ast_node_type	redir_type;
	t_ast_node		*redir_node;
	char			**file_tokens;

	(*last_command)->is_last = is_last;
	if ((*tokens)->type == TOKEN_REDIR_OUT)
		redir_type = AST_REDIR_OUT;
	else if ((*tokens)->type == TOKEN_REDIR_OUT_APPEND)
		redir_type = AST_REDIR_OUT_APPEND;
	else if ((*tokens)->type == TOKEN_REDIR_IN)
		redir_type = AST_REDIR_IN;
	else if ((*tokens)->type == TOKEN_HEREDOC)
		redir_type = AST_HEREDOC;
	else
		return (NULL);
	redir_node = new_ast_node(redir_type, NULL);
	redir_node->left = *root;
	*tokens = (*tokens)->next;
	if ((*tokens)->type == TOKEN_COMMAND)
	{
		file_tokens = extract_command_tokens(tokens);
		redir_node->right = new_ast_node(AST_COMMAND, file_tokens);
	}
	return (redir_node);
}
