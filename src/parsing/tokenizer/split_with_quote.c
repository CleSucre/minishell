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

static void	handle_quote_char(const char *input, int *i, t_tokenizer *tokenizer)
{
	if (tokenizer->quote_char == 0)
		tokenizer->quote_char = input[*i];
	else if (tokenizer->quote_char == input[*i])
		tokenizer->quote_char = 0;
	tokenizer->buffer[tokenizer->buf_pos++] = input[(*i)++];
}

static void	handle_operator_char(const char *input,
						int *i, t_tokenizer *tokenizer)
{
	int	op_len;

	op_len = is_operator_split(input, *i, tokenizer->quote_char);
	token_buffer(tokenizer);
	ft_strncpy(tokenizer->buffer, input + *i, op_len);
	tokenizer->buffer[op_len] = '\0';
	add_token(&tokenizer->tokens,
		&tokenizer->token_count, tokenizer->buffer);
	*i += op_len;
}

static void	process_input(const char *input, int len, t_tokenizer *tokenizer)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (ft_isspace(input[i]) && tokenizer->quote_char == 0)
		{
			token_buffer(tokenizer);
			i++;
		}
		else if (input[i] == '\'' || input[i] == '"')
			handle_quote_char(input, &i, tokenizer);
		else if (is_operator_split(input, i, tokenizer->quote_char))
			handle_operator_char(input, &i, tokenizer);
		else
			tokenizer->buffer[tokenizer->buf_pos++] = input[i++];
	}
	token_buffer(tokenizer);
}

char	**split_with_quotes(const char *input, int *count)
{
	int			len;
	t_tokenizer	tokenizer;

	len = ft_strlen(input);
	tokenizer = init_tokenizer_split(input);
	if (!tokenizer.buffer)
		return (NULL);
	process_input(input, len, &tokenizer);
	free(tokenizer.buffer);
	*count = tokenizer.token_count;
	return (tokenizer.tokens);
}
