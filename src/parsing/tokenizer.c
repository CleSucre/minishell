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
 * @brief Get the type of token from string
 *
 * @param char *str
 * @return t_type
 */
t_type	token_type(char *str)
{
    if (ft_strcmp(str, "|") == 0)
        return (PIPE);
    else if (ft_strcmp(str, "<") == 0)
        return (REDIRECT_IN);
    else if (ft_strcmp(str, ">") == 0)
        return (REDIRECT_OUT);
    else if (ft_strcmp(str, ">>") == 0)
        return (REDIRECT_APPEND);
    else if (1)
        return (COMMAND);
    else
        return (ARGUMENT);
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
	char	**words_start;
	char	**words;

	tokens = NULL;
	words = ft_split(input, WHITESPACES);
	if (!words)
		return (NULL);
	words_start = words;
    while (*words)
	{
        t_token *new_token = create_token(token_type(*words), *words);
        if (!new_token)
            return (NULL);
		free(*words);
        new_token->next = tokens;
        tokens = new_token;
		words++;
    }
	free(words_start);
    return (tokens);
}
