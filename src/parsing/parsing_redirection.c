/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 17:47:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/17 17:47:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
 * @brief Extract the command tokens from the token list.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @return char** Array of strings containing the command tokens.
 */
static t_ast_node	*pre_process_heredoc(t_token **tokens,
				t_ast_node **last_command, int is_last)
{
	t_ast_node		*last_node;

	if (*last_command != NULL)
	{
		if ((*last_command)->type == AST_HEREDOC)
		{
			(*last_command)->is_last = 0;
		}
		else
		{
			(*last_command)->is_last = is_last;
			if ((*last_command)->type == AST_PIPE)
				(*last_command)->right->is_last = is_last;
		}
	}
	last_node = new_ast_node(AST_HEREDOC, NULL);
	*tokens = (*tokens)->next;
	if ((*tokens) == NULL)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error: expected file name\n");
		return (NULL);
	}
	return (last_node);
}

/**
 * @brief Process a heredoc token (<<) and create an AST heredoc node.
 * 			If the current heredoc is not the last one, keep building the AST
 * 			utils the last heredoc is reached. Then, build the ast
 * 			from the last heredoc to the root.
 *
 * @param t_token **tokens
 * @param t_ast_node **root
 * @param t_ast_node **last_command
 * @return int 1 if the function succeeded, 0 otherwise.
 */
int	process_heredoc(t_token **tokens, t_ast_node **root,
				t_ast_node **last_command, int is_last)
{
	t_ast_node		*last_node;
	char			**file_tokens;

	last_node = pre_process_heredoc(tokens, last_command, is_last);
	if (last_node == NULL)
		return (0);
	if ((*tokens)->type == TOKEN_COMMAND)
	{
		file_tokens = extract_command_tokens(tokens);
		last_node->right = new_ast_node(AST_COMMAND, file_tokens);
		if (*tokens)
		{
			if ((*tokens)->type == TOKEN_HEREDOC)
			{
				if (!process_heredoc(tokens, root, &last_node, 1))
					return (0);
			}
		}
	}
	last_node->left = *root;
	*root = last_node;
	return (1);
}

/**
 * @brief Process a redirection token (>, >>, <)
 * 			and create an AST redirection node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param t_ast_node **root Root of the AST being constructed.
 * @return int 1 if the function succeeded, 0 otherwise.
 */
int	process_redirection(t_token **tokens, t_ast_node **root,
					t_ast_node **last_command, int is_last)
{
	t_ast_node		*redir_node;
	char			**file_tokens;
	t_token			*tmp;

	if (*last_command != NULL)
		(*last_command)->is_last = is_last;
	redir_node = new_ast_node(get_redir_type((*tokens)->type), NULL);
	redir_node->left = *root;
	tmp = *tokens;
	*tokens = (*tokens)->next;
	if ((*tokens) == NULL)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error: expected file name\n");
		return (0);
	}
	if ((*tokens)->type == TOKEN_COMMAND)
	{
		if (tmp->type == TOKEN_REDIR_OUT)
			file_tokens = extract_command_tokens(tokens);
		else
		{
			file_tokens = ft_tabnew(1);
			file_tokens[0] = ft_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
			if (*tokens && (*tokens)->type == TOKEN_COMMAND)
				build_ast(tokens, &redir_node->left, last_command);
		}
		redir_node->right = new_ast_node(AST_COMMAND, file_tokens);
	}
	else
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: syntax error: expected file name\n");
		return (0);
	}
	*root = redir_node;
	return (1);
}
