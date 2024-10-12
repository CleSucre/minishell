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
 * @brief The logic suite of the function build_ast.
 * 			Thx norminette !
 *
 * @param t_token **current
 * @param t_ast_node **root
 * @param t_ast_node **last_command
 * @return int
 */
static int build_ast_secondary(t_token **current, t_ast_node **root, t_ast_node **last_command)
{
	if ((*current)->type == TOKEN_COMMAND)
	{
		process_command(current, root, last_command);
		return (1);
	}
	else if ((*current)->type == TOKEN_ARGUMENT)
	{
		process_argument(current, *last_command);
		return (1);
	}
	else if ((*current)->type == TOKEN_PARENTHESIS_OPEN)
	{
		process_subshell(current, root, last_command);
		return (1);
	}
	return (0);
}

/**
 * @brief Generate the AST from the list of tokens given.
 *
 * @param t_token **tokens List of tokens to build the node from.
 * @return t_ast_node* The generated AST node.
 */
t_ast_node	*build_ast(t_token **tokens)
{
	t_ast_node	*root;
	t_ast_node	*last_command;

	root = NULL;
	last_command = NULL;
	while ((*tokens) != NULL)
	{
		if ((*tokens)->type == TOKEN_PARENTHESIS_CLOSE)
			return (root);
		else if ((*tokens)->type == TOKEN_PIPE)
			return (process_pipe(tokens, &root, &last_command));
		else if ((*tokens)->type == TOKEN_AND_OPERATOR
				 || (*tokens)->type == TOKEN_OR_OPERATOR)
			return (process_operator(tokens, &root, &last_command));
		else if ((*tokens)->type == TOKEN_REDIR_OUT
				 || (*tokens)->type == TOKEN_REDIR_OUT_APPEND)
			root = process_redirection(tokens, &root, &last_command, 0);
		else if ((*tokens)->type == TOKEN_REDIR_IN
				 || (*tokens)->type == TOKEN_HEREDOC)
			root = process_redirection(tokens, &root, &last_command, 1);
		else if (!build_ast_secondary(tokens, &root, &last_command))
			*tokens = (*tokens)->next;
	}
	return (root);
}

/**
 * @brief Parses the input string and creates an AST.
 *
 * @param t_minishell *minishell The minishell structure.
 * @param char *input The input string to parse.
 * @return t_ast * AST created from the input.
 */
t_ast_node	*parse_input(t_minishell *minishell, char *input)
{
	t_ast_node	*ast;
	t_token		*tokens;
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
