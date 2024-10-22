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

int	is_operator(const char *input, int i, char quote_char)
{
	if (quote_char != 0)
		return (0);
	if (input[i] == '|' && input[i + 1] == '|')
		return (2);
	else if (input[i] == '&' && input[i + 1] == '&')
		return (2);
	else if (input[i] == '<' && input[i + 1] == '<')
		return (2);
	else if (input[i] == '>' && input[i + 1] == '>')
		return (2);
	else if (input[i] == '<' || input[i] == '>')
		return (1);
	else if (input[i] == '|' || input[i] == '('
		|| input[i] == ')')
		return (1);
	return (0);
}

char	**split_with_quotes(const char *input, int *count)
{
	int		len;
	char	**tokens;
	int		token_count;
	char	*buffer;
	int		buf_pos;
	int		i;
	char	quote_char;
	int		op_len;

	len = ft_strlen(input);
	tokens = NULL;
	token_count = 0;
	buffer = (char *)malloc(len + 1);
	if (buffer == NULL)
		return (NULL);
	buf_pos = 0;
	i = 0;
	quote_char = 0;
	while (i < len)
	{
		if (ft_isspace(input[i]) && quote_char == 0)
		{
			if (buf_pos > 0)
			{
				buffer[buf_pos] = '\0';
				add_token(&tokens, &token_count, buffer);
				buf_pos = 0;
			}
			i++;
		}
		else if (input[i] == '\'' || input[i] == '"')
		{
			if (quote_char == 0)
				quote_char = input[i];
			else if (quote_char == input[i])
				quote_char = 0;
			buffer[buf_pos++] = input[i];
			i++;
		}
		else if (is_operator(input, i, quote_char))
		{
			if (buf_pos > 0)
			{
				buffer[buf_pos] = '\0';
				add_token(&tokens, &token_count, buffer);
				buf_pos = 0;
			}
			op_len = is_operator(input, i, quote_char);
			strncpy(buffer, input + i, op_len);
			buffer[op_len] = '\0';
			add_token(&tokens, &token_count, buffer);
			i += op_len;
		}
		else
		{
			buffer[buf_pos++] = input[i++];
		}
	}
	if (buf_pos > 0)
	{
		buffer[buf_pos] = '\0';
		add_token(&tokens, &token_count, buffer);
	}
	free(buffer);
	*count = token_count;
	return (tokens);
}

int	are_parentheses_valid(char **tokens, int token_count)
{
	int	i;
	int	parentheses_balance;

	i = 0;
	parentheses_balance = 0;
	while (i < token_count)
	{
		if (strcmp(tokens[i], "(") == 0)
		{
			parentheses_balance++;
			if (i + 1 < token_count && strcmp(tokens[i + 1], ")") == 0)
			{
				printf("syntax error near unexpected token `)'\n");
				return (0);
			}
		}
		else if (strcmp(tokens[i], ")") == 0)
		{
			parentheses_balance--;
			if (parentheses_balance < 0)
			{
				printf("syntax error near unexpected token `)'\n");
				return (0);
			}
		}
		i++;
	}
	if (parentheses_balance != 0)
	{
		printf("syntax error: unclosed parentheses\n");
		return (0);
	}
	return (1);
}

void	tokenize(const char *input, t_token **token_list)
{
	int		token_count;
	char	**tokens;
	int		i;

	token_count = 0;
	tokens = split_with_quotes(input, &token_count);
	if (!are_parentheses_valid(tokens, token_count))
	{
		*token_list = NULL;
		i = 0;
		while (i < token_count)
			free(tokens[i++]);
		free(tokens);
		return ;
	}
	i = 0;
	while (i < token_count)
	{
		if (ft_strcmp(tokens[i], "&&") == 0)
			add_token_to_list(token_list, TOKEN_AND_OPERATOR, tokens[i]);
		else if (ft_strcmp(tokens[i], "||") == 0)
			add_token_to_list(token_list, TOKEN_OR_OPERATOR, tokens[i]);
		else if (ft_strcmp(tokens[i], "<<") == 0)
			add_token_to_list(token_list, TOKEN_HEREDOC, tokens[i]);
		else if (ft_strcmp(tokens[i], ">>") == 0)
			add_token_to_list(token_list, TOKEN_REDIR_OUT_APPEND, tokens[i]);
		else if (ft_strcmp(tokens[i], ">") == 0)
			add_token_to_list(token_list, TOKEN_REDIR_OUT, tokens[i]);
		else if (ft_strcmp(tokens[i], "<") == 0)
			add_token_to_list(token_list, TOKEN_REDIR_IN, tokens[i]);
		else if (ft_strcmp(tokens[i], "|") == 0)
			add_token_to_list(token_list, TOKEN_PIPE, tokens[i]);
		else if (ft_strcmp(tokens[i], "(") == 0)
			add_token_to_list(token_list, TOKEN_PARENTHESIS_OPEN, tokens[i]);
		else if (ft_strcmp(tokens[i], ")") == 0)
			add_token_to_list(token_list, TOKEN_PARENTHESIS_CLOSE, tokens[i]);
		else
			add_token_to_list(token_list, TOKEN_COMMAND, tokens[i]);
		i++;
	}
	i = 0;
	while (i < token_count)
		free(tokens[i++]);
	free(tokens);
}

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
