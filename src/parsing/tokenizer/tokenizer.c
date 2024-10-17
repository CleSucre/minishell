/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Identify the type of token from the string
 *
 * @param char *str The string to analyze
 * @return t_token_type The type of the token
 */
t_token_type	token_type(char *str)
{
	if (ft_strcmp(str, "&&") == 0)
		return (TOKEN_AND_OPERATOR);
	else if (ft_strcmp(str, "||") == 0)
		return (TOKEN_OR_OPERATOR);
	else if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(str, ">") == 0)
		return (TOKEN_REDIR_OUT);
	else if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_REDIR_OUT_APPEND);
	else if (ft_strcmp(str, "<") == 0)
		return (TOKEN_REDIR_IN);
	else if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	else if (ft_strncmp(str, "(", 1) == 0)
		return (TOKEN_PARENTHESIS_OPEN);
	else if (ft_strncmp(str, ")", 1) == 0)
		return (TOKEN_PARENTHESIS_CLOSE);
	else
		return (TOKEN_COMMAND);
}

/**
 * @brief Tokenize the input string
 *
 * @param char	*input
 * @return t_token*
 */
t_token	*tokenize(char *input)
{
	t_token			*head;
	int				i;
	char			*token_value;
	t_token_type	type;
	t_token			*token;

	head = NULL;
	i = 0;
	while (input[i] != '\0')
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		token_value = extract_token(input, &i);
		if (token_value == NULL)
			return (NULL);
		type = token_type(token_value);
		token = new_token(token_value, type);
		add_token(&head, token);
		free(token_value);
	}
	return (head);
}

/**
 * @brief Extract the command tokens from the token list
 *
 * @param t_token **tokens
 * @return char**
 */
char	**extract_command_tokens(t_token **tokens)
{
	char	**command_tokens;
	int		token_count;
	t_token	*current;

	command_tokens = (char **)malloc(MAX_COMMAND_SIZE * sizeof(char *));
	if (command_tokens == NULL)
		return (NULL);
	token_count = 0;
	current = *tokens;
	while (current != NULL
		&& (current->type == TOKEN_COMMAND || current->type == TOKEN_ARGUMENT))
	{
		command_tokens[token_count++] = ft_strdup(current->value);
		current = current->next;
	}
	command_tokens[token_count] = NULL;
	*tokens = current;
	return (command_tokens);
}
