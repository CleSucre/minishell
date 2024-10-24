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
	if ((*current)->type == TOKEN_REDIR_IN)
		return (process_redirection(current, root, last_command, 1));
	else if ((*current)->type == TOKEN_HEREDOC)
		return (process_heredoc(current, root, last_command, 1));
	else if ((*current)->type == TOKEN_COMMAND)
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
	if (!current)
		return (1);
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
 * @brief Tokenize the validated input.
 *
 * @param t_minishell *minishell
 * @param char *trimmed the validated and trimmed input
 * @return t_token* the list of tokens
 */
static t_token	*tokenize_input(t_minishell *minishell, char *trimmed)
{
	t_token	*tokens;

	tokens = NULL;
	tokenize(trimmed, &tokens);
	free(trimmed);
	if (!tokens)
	{
		minishell->exit_code = 2;
		return (NULL);
	}
	debug_tokens(tokens);
	return (tokens);
}

/**
 * @brief Check the input and tokenize it.
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return t_token* the list of tokens
 */
static t_token	*check_and_tokenize_input(t_minishell *minishell, char *input)
{
	char	*trimmed;

	trimmed = check_input_and_validate(minishell, input);
	if (!trimmed)
		return (NULL);
	return (tokenize_input(minishell, trimmed));
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
	int			error;

	tokens = check_and_tokenize_input(minishell, input);
	if (!tokens)
		return (NULL);
	if (token_is_logic_operator(tokens))
	{
		ft_fprintf(STDERR_FILENO, "syntax error near unexpected token '%s'\n",
			tokens->value);
		free_tokens(tokens);
		minishell->exit_code = 2;
		return (NULL);
	}
	ast = NULL;
	last_command = NULL;
	error = build_ast(&tokens, &ast, &last_command);
	debug_ast(ast);
	if (!handle_token_errors(minishell, tokens, error, ast))
		return (NULL);
	return (ast);
}
