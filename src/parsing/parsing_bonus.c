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
