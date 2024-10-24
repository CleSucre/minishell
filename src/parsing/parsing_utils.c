/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 04:02:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/19 04:02:00 by julthoma         ###   ########.fr       */
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
char	*check_input(char *input)
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
	return (trimmed);
}

/**
 * @brief Check if simple and double quotes are correctly closed
 * 			in respectives order.
 * 			If a ' is between two ", it is not counted.
 * 			Same for ".
 *
 * @param char *str
 * @return int 1 if the quotes are correctly closed, 0 otherwise
 */
int	check_quotes_count(char *str)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && double_quote == 0)
			single_quote = !single_quote;
		else if (str[i] == '"' && single_quote == 0)
			double_quote = !double_quote;
		i++;
	}
	if (single_quote || double_quote)
		return (0);
	return (1);
}

/**
 * @brief Check and validate the input.
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return char* the trimmed input or NULL if invalid
 */
char	*check_input_and_validate(t_minishell *minishell, char *input)
{
	char	*trimmed;

	trimmed = check_input(input);
	if (!input || !trimmed)
		return (NULL);
	if (ft_isprint(*trimmed))
		history_add(minishell, trimmed, 1);
	if (check_quotes_count(trimmed) == 0)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error: unexpected EOF\n");
		minishell->exit_code = 2;
		free(trimmed);
		return (NULL);
	}
	return (trimmed);
}

/**
 * @brief Handle the token errors.
 *
 * @param t_minishell *minishell
 * @param t_token *tokens
 * @param int error
 * @param t_ast_node *ast
 * @return int 1 if the function succeeded, 0 otherwise.
 */
int	handle_token_errors(t_minishell *minishell,
							t_token *tokens, int error, t_ast_node *ast)
{
	if (error == 0)
	{
		minishell->exit_code = 2;
		free_tokens(tokens);
		free_ast(ast);
		return (0);
	}
	else if (error == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error: expected '('\n");
		free_tokens(tokens);
		free_ast(ast);
		return (0);
	}
	free_tokens(tokens);
	return (1);
}
