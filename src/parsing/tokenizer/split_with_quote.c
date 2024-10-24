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
//
//static int	is_operator(const char *input, int i, char quote_char)
//{
//	if (quote_char != 0)
//		return (0);
//	if (input[i] == '|' && input[i + 1] == '|')
//		return (2);
//	else if (input[i] == '&' && input[i + 1] == '&')
//		return (2);
//	else if (input[i] == '<' && input[i + 1] == '<')
//		return (2);
//	else if (input[i] == '>' && input[i + 1] == '>')
//		return (2);
//	else if (input[i] == '<' || input[i] == '>')
//		return (1);
//	else if (input[i] == '|' || input[i] == '('
//		|| input[i] == ')')
//		return (1);
//	return (0);
//}
//
//static void	token_buffer(int *buf_pos, char *buffer,
//							char ***tokens, int *token_count)
//{
//	if (*buf_pos > 0)
//	{
//		buffer[*buf_pos] = '\0';
//		add_token(tokens, token_count, buffer);
//		*buf_pos = 0;
//	}
//}
//
//char	**split_with_quotes(const char *input, int *count)
//{
//	int		len;
//	char	**tokens;
//	int		token_count;
//	char	*buffer;
//	int		buf_pos;
//	int		i;
//	char	quote_char;
//	int		op_len;
//
//	len = ft_strlen(input);
//	tokens = NULL;
//	token_count = 0;
//	buffer = (char *)malloc(len + 1);
//	if (buffer == NULL)
//		return (NULL);
//	i = 0;
//	buf_pos = 0;
//	quote_char = 0;
//	while (i < len)
//	{
//		if (ft_isspace(input[i]) && quote_char == 0)
//		{
//			token_buffer(&buf_pos, buffer, &tokens, &token_count);
//			i++;
//		}
//		else if (input[i] == '\'' || input[i] == '"')
//		{
//			if (quote_char == 0)
//				quote_char = input[i];
//			else if (quote_char == input[i])
//				quote_char = 0;
//			buffer[buf_pos++] = input[i];
//			i++;
//		}
//		else if (is_operator(input, i, quote_char))
//		{
//			token_buffer(&buf_pos, buffer, &tokens, &token_count);
//			op_len = is_operator(input, i, quote_char);
//			ft_strncpy(buffer, input + i, op_len);
//			buffer[op_len] = '\0';
//			add_token(&tokens, &token_count, buffer);
//			i += op_len;
//		}
//		else
//			buffer[buf_pos++] = input[i++];
//	}
//	token_buffer(&buf_pos, buffer, &tokens, &token_count);
//	free(buffer);
//	*count = token_count;
//	return (tokens);
//}
//
