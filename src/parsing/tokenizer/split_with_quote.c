/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <pierrot.maxime.pro@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 01:39:24 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/24 03:02:13 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	token_buffer(t_tokenizer *tokenizer)
{
	if (tokenizer->buf_pos > 0)
	{
		tokenizer->buffer[tokenizer->buf_pos] = '\0';
		add_token(&tokenizer->tokens,
			&tokenizer->token_count, tokenizer->buffer);
		tokenizer->buf_pos = 0;
	}
}

static void	handle_space(const char *input, int *i, t_tokenizer *tokenizer)
{
	if (ft_isspace(input[*i]) && tokenizer->quote_char == 0)
	{
		token_buffer(tokenizer);
		(*i)++;
	}
}

static void	process_quote(const char *input, int *i, t_tokenizer *tokenizer)
{
	if (input[*i] == '\'' || input[*i] == '"')
	{
		if (tokenizer->quote_char == 0)
			tokenizer->quote_char = input[*i];
		else if (tokenizer->quote_char == input[*i])
			tokenizer->quote_char = 0;
		tokenizer->buffer[tokenizer->buf_pos++] = input[*i];
		(*i)++;
	}
}

static void	handle_operator(const char *input, int *i, t_tokenizer *tokenizer)
{
	int	op_len;

	if (is_operator_split(input, *i, tokenizer->quote_char))
	{
		token_buffer(tokenizer);
		op_len = is_operator_split(input, *i, tokenizer->quote_char);
		ft_strncpy(tokenizer->buffer, input + *i, op_len);
		tokenizer->buffer[op_len] = '\0';
		add_token(&tokenizer->tokens,
			&tokenizer->token_count, tokenizer->buffer);
		*i += op_len;
	}
}

char	**split_with_quotes(const char *input, int *count)
{
	int			i;
	int			len;
	t_tokenizer	tokenizer;

	len = ft_strlen(input);
	tokenizer = init_tokenizer_split(input);
	if (!tokenizer.buffer)
		return (NULL);
	i = 0;
	while (i < len)
	{
		handle_space(input, &i, &tokenizer);
		process_quote(input, &i, &tokenizer);
		handle_operator(input, &i, &tokenizer);
		if (!ft_isspace(input[i]) && !(input[i] == '\'' || input[i] == '"')
			&& !is_operator_split(input, i, tokenizer.quote_char))
			tokenizer.buffer[tokenizer.buf_pos++] = input[i++];
	}
	token_buffer(&tokenizer);
	free(tokenizer.buffer);
	*count = tokenizer.token_count;
	return (tokenizer.tokens);
}

//char **split_with_quotes(const char *input, int *count)
//{
//	int			len;
//	int			i;
//	int			op_len;
//	t_tokenizer	tokenizer;
//
//	len = ft_strlen(input);
//	tokenizer.tokens = NULL;
//	tokenizer.token_count = 0;
//	tokenizer.buffer = (char *)malloc(len + 1);
//	if (tokenizer.buffer == NULL)
//		return (NULL);
//	tokenizer.buf_pos = 0;
//	tokenizer.quote_char = 0;
//	i = 0;
//	while (i < len)
//	{
//		if (ft_isspace(input[i]) && tokenizer.quote_char == 0)
//		{
//			token_buffer(&tokenizer);
//			i++;
//		}
//		else if (input[i] == '\'' || input[i] == '"')
//		{
//			if (tokenizer.quote_char == 0)
//				tokenizer.quote_char = input[i];
//			else if (tokenizer.quote_char == input[i])
//				tokenizer.quote_char = 0;
//			tokenizer.buffer[tokenizer.buf_pos++] = input[i];
//			i++;
//		}
//		else if (is_operator_split(input, i, tokenizer.quote_char))
//		{
//			token_buffer(&tokenizer);
//			op_len = is_operator_split(input, i, tokenizer.quote_char);
//			ft_strncpy(tokenizer.buffer, input + i, op_len);
//			tokenizer.buffer[op_len] = '\0';
//			add_token(&tokenizer.tokens,
//				&tokenizer.token_count, tokenizer.buffer);
//			i += op_len;
//		}
//		else
//			tokenizer.buffer[tokenizer.buf_pos++] = input[i++];
//	}
//
//	token_buffer(&tokenizer);
//	free(tokenizer.buffer);
//	*count = tokenizer.token_count;
//	return (tokenizer.tokens);
//}
