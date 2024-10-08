/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:27:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/11 19:27:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check the following things:
 * 	- If the input is empty, print a new line and return NULL
 * 	- Trim the input
 * 	- Add the input to the history
 * 	- Print the input in debug mode if needed
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return char* the trimmed input
 */
static char	*check_input(t_minishell *minishell, char *input)
{
	char	*trimmed;

	if (ft_strlen(input) == 0)
	{
		terminal_print("", 1, STDOUT_FILENO);
		free(input);
		return (NULL);
	}
	trimmed = ft_strtrim(input, WHITESPACES);
	free(input);
	if (!trimmed)
		return (NULL);
	debug_execution(trimmed);
	if (ft_isprint(*trimmed))
		history_add(minishell, trimmed, 1);
	return (trimmed);
}

/**
 * @brief Build a new AST node based on tokens given.
 *
 * @param t_token **tokens List of tokens to build the node from.
 * @return t_ast_node* New AST node created.
 */
t_ast_node	*build_ast(t_token **tokens)
{
	t_token *current;
	t_ast_node *root;
	t_ast_node *last_command;

	current = *tokens;
	root = NULL;
	last_command = NULL;
	while (current != NULL)
	{
		if (current->type == TOKEN_COMMAND)
			process_command(&current, &root, &last_command);
		else if (current->type == TOKEN_ARGUMENT)
		{
			process_argument(current, last_command);
			current = current->next;
			continue ;
		}
		else if (current->type == TOKEN_PIPE)
			return process_pipe(&current, &root);
		else if (current->type == TOKEN_AND_OPERATOR || current->type == TOKEN_OR_OPERATOR)
			return process_operator(&current, &root, &last_command);
		else if (current->type == TOKEN_PARENTHESIS_OPEN)
			process_subshell(&current, &root, &last_command);
		else if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_OUT_APPEND ||
				 current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
			root = process_redirection(&current, &root);
		else if (current->type == TOKEN_ASSIGNMENT)
		{
			t_ast_node *assignment_node = process_assignment(&current);
			if (root == NULL)
				root = assignment_node;
			else
				last_command->right = assignment_node;
		}
		else if (current->type == TOKEN_VARIABLE)
		{
			t_ast_node *variable_node = process_variable(&current);
			if (root == NULL)
				root = variable_node;
			else
				last_command->right = variable_node;
		}
		else
			current = current->next;
	}
	return (root);
}


/**
 * @brief Parses the input string and creates an AST.
 *
 * @param char *input Input string to parse.
 * @return t_ast * AST created from the input.
 */
t_ast_node	*parse_input(t_minishell *minishell, char *input)
{
	t_ast_node	*ast;
    t_token     *tokens;
	char		*trimmed;

	if (!input)
		return (NULL);
	trimmed = check_input(minishell, input);
	if (!trimmed)
		return (0);
    tokens = tokenize(trimmed);
	free(trimmed);
    if (!tokens)
        return (NULL);
    debug_tokens(tokens);
    ast = build_ast(&tokens);
	free_tokens(tokens);
	if (!ast)
		return (NULL);
	debug_ast(ast);
	return (ast);
}
