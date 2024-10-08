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
 * @brief Process a command token and create an AST command node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param t_ast_node **root Root of the AST being constructed.
 * @param t_ast_node **last_command Last command processed (to attach arguments or commands).
 */
void	process_command(t_token **tokens, t_ast_node **root, t_ast_node **last_command)
{
	char		**command_tokens;
	t_ast_node	*command_node;

	command_tokens = extract_command_tokens(tokens);
	command_node = new_ast_node(AST_COMMAND, command_tokens);
	if (!command_tokens || !command_node)
		return ;
	if (*last_command == NULL)
		*root = command_node;
	else if ((*last_command)->right == NULL)
		(*last_command)->right = command_node;
	*last_command = command_node;
}

/**
 * @brief Process an argument token and attach it to the last command node.
 *
 * @param t_token *current Current token (argument).
 * @param t_ast_node *last_command Last command node.
 */
void	process_argument(t_token *current, t_ast_node *last_command)
{
	int	i;

	if (last_command && last_command->type == AST_COMMAND)
	{
		i = 0;
		while (last_command->value[i])
			i++;
		last_command->value[i] = strdup(current->value);
		last_command->value[i + 1] = NULL;
	}
}

/**
 * @brief Process a pipe token and create an AST pipe node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @param t_ast_node **root Root of the AST being constructed.
 * @return t_ast_node* The pipe node created.
 */
t_ast_node	*process_pipe(t_token **tokens, t_ast_node **root)
{
	t_ast_node *pipe_node;

	pipe_node = new_ast_node(AST_PIPE, NULL);
	pipe_node->left = *root;
	*tokens = (*tokens)->next;
	pipe_node->right = build_ast(tokens);
	return (pipe_node);
}

/**
 * @brief Process an assignment token and create an AST assignment node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @return t_ast_node* The assignment node created.
 */
t_ast_node	*process_assignment(t_token **tokens)
{
	char	**assignment_tokens;

	assignment_tokens = extract_command_tokens(tokens);
	return (new_ast_node(AST_ASSIGNMENT, assignment_tokens));
}

/**
 * @brief Process a variable token and create an AST variable node.
 *
 * @param t_token **tokens Pointer to the current list of tokens.
 * @return t_ast_node* The variable node created.
 */
t_ast_node	*process_variable(t_token **tokens)
{
	char	**variable_tokens;

	variable_tokens = extract_command_tokens(tokens);
	return (new_ast_node(AST_VARIABLE, variable_tokens));
}
