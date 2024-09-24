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
 * @param char *str
 * @return t_token_type
 */
t_token_type	token_type(char *str)
{
	if (str[0] == '$')
		return TOKEN_VARIABLE;  // Variable référencée dans une commande
	else if (ft_strchr(str, '=') != NULL)
		return (TOKEN_ASSIGNMENT);  // Assignation de variable
	else if (ft_strcmp(str, "&&") == 0)
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
    else if (ft_strncmp(str, "(", 1) == 0)
        return (TOKEN_PARENTHESIS_OPEN);
    else if (ft_strncmp(str, ")", 1) == 0)
        return (TOKEN_PARENTHESIS_CLOSE);
    else
        return (TOKEN_COMMAND);
}

/**
 * @brief Manage quoted tokens
 *
 * @param char *input
 * @param int *index
 * @return char*
 */
char	*extract_quoted_token(char *input, int *index)
{
	char	buffer[1024];
	int		buffer_pos;
	char	quote_type;

	buffer_pos = 0;
	quote_type = input[*index];
	(*index)++;
	while (input[*index] != quote_type && input[*index] != '\0')
	{
		buffer[buffer_pos++] = input[*index];
		(*index)++;
	}
	if (input[*index] == quote_type)
		(*index)++;
	buffer[buffer_pos] = '\0';
	return (ft_strdup(buffer));
}

/**
 * @brief Manage quotes and parentheses
 *
 * @param char *input
 * @param int *index
 * @return char*
 */
char	*extract_token(char *input, int *index)
{
	char	buffer[1024];
	int		buffer_pos;
	char	current_char;

	buffer_pos = 0;
	current_char = input[*index];
	if (current_char == '"' || current_char == '\'')
		return (extract_quoted_token(input, index));
	if (current_char == '(' || current_char == ')')
	{
		buffer[0] = current_char;
		buffer[1] = '\0';
		(*index)++;
		return (ft_strdup(buffer));
	}
	while (input[*index] != '\0' && !ft_isspace(input[*index]) && input[*index] != '(' && input[*index] != ')')
	{
		buffer[buffer_pos++] = input[*index];
		(*index)++;
	}
	buffer[buffer_pos] = '\0';
	return (ft_strdup(buffer));
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
        type = token_type(token_value);
        t_token *token = new_token(token_value, type);
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
	char **command_tokens;
	int token_count;
	t_token *current;

	command_tokens = (char **)malloc(MAX_COMMAND_SIZE * sizeof(char *));
	if (command_tokens == NULL)
		return (NULL);
	token_count = 0;
	current = *tokens;
	while (current != NULL &&
			(current->type == TOKEN_COMMAND || current->type == TOKEN_ARGUMENT))
	{
		command_tokens[token_count++] = ft_strdup(current->value);
		current = current->next;
	}
	command_tokens[token_count] = NULL;
	*tokens = current;
	return (command_tokens);
}
