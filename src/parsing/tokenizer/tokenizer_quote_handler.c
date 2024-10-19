/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quote_handler.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Manage quoted tokens
 *
 * @param char *input The input string
 * @param int *index The current index in the input string
 * @return char * The extracted token
 */
char	*extract_quoted_token(char *input, int *index)
{
	char	buffer[1024];
	int		buffer_pos;
	char	quote_type;

	buffer_pos = 0;
	if (input[*index] == '"' || input[*index] == '\'')
	{
		quote_type = input[*index];
		(*index)++;
	}
	else
		return (NULL);
	while (input[*index] != quote_type && input[*index] != '\0')
	{
		buffer[buffer_pos++] = input[*index];
		(*index)++;
	}
	if (input[*index] != quote_type)
	{
		ft_fprintf(STDERR_FILENO, "minishell: syntax error: unclosed quote\n");
		return (NULL);
	}
	(*index)++;
	buffer[buffer_pos] = '\0';
	return (ft_strdup(buffer));
}

/**
 * @brief Handle quoted tokens
 *
 * @param char *buffer The buffer containing the current token
 * @param int *buffer_pos The current position in the buffer
 * @param char *input The input string
 * @param int *index The current index in the input string
 * @return char* The final token
 */
char	*handle_quotes(char *buffer, int *buffer_pos,
						char *input, int *index)
{
	char	*quoted_token;
	char	*temp_token;
	char	*final_token;

	quoted_token = extract_quoted_token(input, index);
	if (!quoted_token)
		return (NULL);
	if (*buffer_pos > 0)
	{
		buffer[*buffer_pos] = '\0';
		temp_token = ft_strdup(buffer);
		final_token = ft_strjoin(temp_token, quoted_token);
		free(temp_token);
		free(quoted_token);
		return (final_token);
	}
	return (quoted_token);
}

/**
 * @brief Handle parentheses
 *
 * @param char current_char The current character
 * @param int *index The current index in the input string
 * @return char* The extracted token
 */
char	*handle_parentheses(char current_char, int *index)
{
	char	buffer[2];

	buffer[0] = current_char;
	buffer[1] = '\0';
	(*index)++;
	return (ft_strdup(buffer));
}
