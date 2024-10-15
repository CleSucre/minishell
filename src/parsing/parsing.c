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
 * @return int 1 if the function succeeded, 0 otherwise.
 */
static int	build_ast_secondary(t_token **current, t_ast_node **root,
							t_ast_node **last_command)
{
	if ((*current)->type == TOKEN_REDIR_IN
		|| (*current)->type == TOKEN_HEREDOC)
		return (process_redirection(current, root, last_command, 1));
	else if ((*current)->type == TOKEN_COMMAND || (*current)->type == TOKEN_VARIABLE)
		return (process_command(current, root, last_command));
	else if ((*current)->type == TOKEN_ARGUMENT)
		return (process_argument(current, *last_command));
	else if ((*current)->type == TOKEN_PARENTHESIS_OPEN)
		return (process_subshell(current, root, last_command));
	else
		*current = (*current)->next;
	return (1);
}

/**
 * @brief Generate the AST from the list of tokens given.
 *
 * @param t_token **tokens List of tokens to build the node from.
 * @return int 1 if the function succeeded, 0 otherwise.
 */
int	build_ast(t_token **tokens, t_ast_node **root, t_ast_node **last_command)
{
	t_token		*current;

	current = *tokens;
	if (current->type == TOKEN_PARENTHESIS_CLOSE)
		return (-1);
	else if (current->type == TOKEN_PIPE)
	{
		return (process_pipe(&current, root, last_command));
	}
	else if (current->type == TOKEN_AND_OPERATOR
		|| current->type == TOKEN_OR_OPERATOR)
	{
		return (process_operator(&current, root, last_command));
	}
	else if (current->type == TOKEN_REDIR_OUT
		|| current->type == TOKEN_REDIR_OUT_APPEND)
		process_redirection(&current, root, last_command, 0);
	else if (!build_ast_secondary(&current, root, last_command))
		return (0);
	if (current)
		return (build_ast(&current, root, last_command));
	return (1);
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
	t_ast_node	*last_command;
	t_token		*tokens;
	char		*trimmed;
	int 		error;

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
	ast = NULL;
	last_command = NULL;
	error = build_ast(&tokens, &ast, &last_command);
	if (error == 0)
	{
		free_tokens(tokens);
		return (NULL);
	}
	else if (error == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error: expected '('\n");
		free_tokens(tokens);
		return (NULL);
	}
	free_tokens(tokens);
	debug_ast(ast);
	return (ast);
}
