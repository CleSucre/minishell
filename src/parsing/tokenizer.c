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
 * @brief Get the type of token from string
 *
 * @param char *str
 * @return t_type
 */
static t_type	token_type(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(str, "<") == 0)
		return (REDIRECT_IN);
	else if (ft_strcmp(str, ">") == 0)
		return (REDIRECT_OUT);
	else if (ft_strcmp(str, ">>") == 0)
		return (REDIRECT_APPEND);
	else if (ft_strncmp(str, "-", 1) == 0)
		return (FLAG);
	else if (ft_strncmp(str, "$", 1) == 0)
		return (VARIABLE);
	else if (ft_strncmp(str, "\"", 1) == 0)
		return (TEXT_DOUBLE_QUOTE);
	else if (ft_strncmp(str, "\'", 1) == 0)
		return (TEXT_SINGLE_QUOTE);
	else
		return (COMMAND);
}

/**
 * @brief Create a token object
 *
 * @param t_type type of token
 * @param char *value value of token
 * @return
 */
t_token	*create_token(t_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	return (token);
}

/**
 * @brief Rejoin words that start and end with quotes and everything in between
 *
 * @param char	**words
 */
static void rejoin_quotes(char **words)
{
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	len;
	char 	*tmp;

	i = 0;
	while (words[i])
	{
		if ((words[i][0] == '\'')
			|| words[i][0] == '\"')
		{
			k = i;
			j = i;
			while (words[j])
			{
				len = ft_strlen(words[j]);
				if (((words[j][len - 1] == '\'' && (words[j][len - 2] != '\\'))
					|| (words[j][len - 1] == '\"') && (words[j][len - 2] != '\\')))
				{
					if (k == j)
						break ;
					while (i < j)
					{
						tmp = ft_strjoin(words[k], " ");
						free(words[k]);
						words[k] = tmp;
						tmp = ft_strjoin(words[k], words[i + 1]);
						free(words[k]);
						free(words[i + 1]);
						words[k] = tmp;
						i++;
					}
					//move the rest of the words to the left
					while (words[j])
					{
						words[k + 1] = words[j + 1];
						j++;
						k++;
					}
					words[k] = NULL;
					i = 0;
					break;
				}
				j++;
			}
		}
		i++;
	}
}

/**
 * @brief Tokenize the input
 *
 * @param char *input
 * @return t_token *
 */
t_token	*tokenize(char *input)
{
	t_token	*tokens;
	t_token	*new_token;
	char	**words;
	int 	words_count;

	tokens = NULL;
	words = ft_split_quote(input, WHITESPACES);
	if (!words)
		return (NULL);
	words_count = (int)ft_strlentab((const char **)words) - 1;
	while (words_count >= 0)
	{
		new_token = create_token(token_type(words[words_count]),
				words[words_count]);
		if (!new_token)
			return (NULL);
		free(words[words_count]);
		new_token->next = tokens;
		tokens = new_token;
		words_count--;
	}
	free(words);
	return (tokens);
}
