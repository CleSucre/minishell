/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_mandatory.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:27:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/11 19:27:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Process a command token and create
 * 			an AST command node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param t_ast_node **root Root of the AST being constructed.
 * @param t_ast_node **last_command Last command processed
 * 					(to attach arguments or commands).
 * @return int 1 if the function succeeded, 0 otherwise.
 */
int	process_command(t_token **tokens, t_ast_node **root,
						t_ast_node **last_command)
{
	char		**command_tokens;
	t_ast_node	*command_node;

	command_tokens = extract_command_tokens(tokens);
	if (!command_tokens)
		return (0);
	command_node = new_ast_node(AST_COMMAND, command_tokens);
	if (!command_node)
		return (0);
	if (*last_command == NULL)
		*root = command_node;
	else if ((*last_command)->right == NULL)
		(*last_command)->right = command_node;
	*last_command = command_node;
	return (1);
}

/**
 * @brief Process an argument token and attach it to the last command node.
 *
 * @param t_token *current Current token (argument).
 * @param t_ast_node *last_command Last command node.
 */
int	process_argument(t_token **current, t_ast_node *last_command)
{
	int	i;

	if (last_command && last_command->type == AST_COMMAND)
	{
		i = 0;
		while (last_command->value[i])
			i++;
		last_command->value[i] = ft_strdup((*current)->value);
		last_command->value[i + 1] = NULL;
	}
	*current = (*current)->next;
	return (1);
}

/**
 * @brief Process a pipe token and create an AST pipe node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param t_ast_node **root Root of the AST being constructed.
 * @return t_ast_node* The pipe node created.
 */
int	process_pipe(t_token **tokens, t_ast_node **root,
						t_ast_node **last_command)
{
	t_ast_node	*pipe_node;
	t_ast_node	*new_last_command;

	if (*last_command)
		(*last_command)->is_last = 0;
	if (!*last_command)
		return (0);
	if (!(*tokens)->next)
		return (0);
	pipe_node = new_ast_node(AST_PIPE, NULL);
	pipe_node->left = *root;
	*tokens = (*tokens)->next;
	new_last_command = NULL;
	*root = pipe_node;
	return (build_ast(tokens, &pipe_node->right, &new_last_command));
}
