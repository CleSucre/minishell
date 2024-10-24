/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <pierrot.maxime.pro@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:18:16 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/24 13:18:16 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokenizer	init_tokenizer_split(const char *input)
{
	t_tokenizer	tokenizer;
	int			len;

	len = ft_strlen(input);
	tokenizer.tokens = NULL;
	tokenizer.token_count = 0;
	tokenizer.buffer = (char *)malloc(len + 1);
	if (tokenizer.buffer == NULL)
		return (tokenizer);
	tokenizer.buf_pos = 0;
	tokenizer.quote_char = 0;
	return (tokenizer);
}

int	is_operator_split(const char *input, int i, char quote_char)
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
