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
 * @brief Count opened parentheses in the current list of tokens.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @return int The number of opened parentheses.
 */
static int	count_open_parentheses(t_token **tokens)
{
	int		open_parens;
	t_token	*tmp;

	open_parens = 0;
	tmp = *tokens;
	while (tmp != NULL && tmp->type == TOKEN_PARENTHESIS_OPEN)
	{
		open_parens++;
		tmp = tmp->next;
	}
	return (open_parens);
}

/**
 * @brief Skip tokens until the corresponding closing
 * 			parenthesis is found.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param int opened Number of opened parentheses.
 * @return int Returns 1 if successful, 0 if an error occurs
 * 			(e.g., unmatched parentheses).
 */
static int	skip_parentheses(t_token **tokens, int opened)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp != NULL && opened > 0)
	{
		if (tmp->type == TOKEN_PARENTHESIS_CLOSE)
			opened--;
		tmp = tmp->next;
	}
	if (opened > 0)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error: expected ')'\n");
		return (0);
	}
	*tokens = tmp;
	return (1);
}

/**
 * @brief Find last AST_COMMAND node and update it to is_last = 0
 *
 * @param t_ast_node **root Root of the AST being constructed.
 */
static void	update_last(t_ast_node **root)
{
	t_ast_node	*tmp;

	tmp = *root;
	while (tmp->right != NULL)
		tmp = tmp->right;
	tmp->is_last = 0;
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
	int			opened;

	if ((*tokens)->prev != NULL && (*tokens)->prev->type == TOKEN_COMMAND)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `('\n");
		return (0);
	}
	subshell_node = new_ast_node(AST_SUBSHELL, NULL);
	if (build_ast(&(*tokens)->next, &subshell_node->left, last_command) != -1)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error: expected ')'\n");
		return (0);
	}
	opened = count_open_parentheses(tokens);
	if (!skip_parentheses(tokens, opened))
		return (0);
	if (*root == NULL)
		*root = subshell_node;
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
		update_last(&subshell_node->left);
	return (1);
}
