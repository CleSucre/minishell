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
 * @brief Manage quoted tokens
 *
 * @param char *input The input string
 * @param int *index The current index in the input string
 * @return char * The extracted token
 */
static char	*extract_quoted_token(char *input, int *index)
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
	if (input[*index] == quote_type)
		(*index)++;
	buffer[buffer_pos] = '\0';
	return (ft_strdup(buffer));
}

static char	*handle_quotes(char *buffer, int *buffer_pos,
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

static char	*handle_parentheses(char current_char, int *index)
{
	char	buffer[2];

	buffer[0] = current_char;
	buffer[1] = '\0';
	(*index)++;
	return (ft_strdup(buffer));
}

static int	is_operator(char *input, int index)
{
	if (input[index] == '|' && input[index + 1] == '|')
		return (2);
	if (input[index] == '&' && input[index + 1] == '&')
		return (2);
	if (input[index] == '|' || input[index] == '&')
		return (1);
	return (0);
}

static char	*handle_operator(char *input, int *index)
{
	char	buffer[3];
	int		op_length;

	op_length = is_operator(input, *index);
	if (op_length > 0)
	{
		ft_strncpy(buffer, &input[*index], op_length);
		buffer[op_length] = '\0';
		*index += op_length;
		return (ft_strdup(buffer));
	}
	return (NULL);
}

static char	*process_token(char *input, int *index,
					char *buffer, int *buffer_pos)
{
	while (input[*index] != '\0' && !ft_isspace(input[*index])
		&& input[*index] != '(' && input[*index] != ')')
	{
		if (input[*index] == '"' || input[*index] == '\'')
			return (handle_quotes(buffer, buffer_pos, input, index));
		if (is_operator(input, *index) > 0)
		{
			if (*buffer_pos > 0)
			{
				buffer[*buffer_pos] = '\0';
				(*buffer_pos) = 0;
				return (ft_strdup(buffer));
			}
			return (handle_operator(input, index));
		}
		buffer[(*buffer_pos)++] = input[*index];
		(*index)++;
	}
	buffer[*buffer_pos] = '\0';
	return (ft_strdup(buffer));
}

/**
 * @brief Manage quotes, parentheses, and operators like &&, ||, |
 *
 * @param char *input The input string
 * @param int *index The current index in the input string
 * @return char*
 */
char	*extract_token(char *input, int *index)
{
	char	buffer[BUFFER_SIZE];
	int		buffer_pos;
	char	current_char;

	buffer_pos = 0;
	current_char = input[*index];
	if (current_char == '"' || current_char == '\'')
		return (extract_quoted_token(input, index));
	if (current_char == '(' || current_char == ')')
		return (handle_parentheses(current_char, index));
	if (is_operator(input, *index) > 0)
		return (handle_operator(input, index));
	return (process_token(input, index, buffer, &buffer_pos));
}
