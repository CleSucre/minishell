/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/10 09:14:14 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Create a new token
 *
 * @param str The value of the token
 * @param type The type of the token
 * @return t_token* The new token
 */
t_token	*new_token(char *str, int type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(str);
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

/**
 * @brief Add a token to the token list
 *
 * @param tokens The head of the token list
 * @param type The type of the token
 * @param value The value of the token
 */
void add_token_to_list(t_token **tokens, t_token_type type, char *value)
{
	t_token *new_token = malloc(sizeof(t_token));
	new_token->type = type;
	new_token->value = strdup(value);
	new_token->next = NULL;

	if (!*tokens)
	{
		*tokens = new_token;
	}
	else
	{
		t_token *temp = *tokens;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
	}
}

/**
 * @brief Add a token to the token list
 *
 * @param head The head of the token list
 * @param new The new token to add
 */

void add_token(char ***tokens, int *token_count, char *token)
{
	*tokens = realloc(*tokens, sizeof(char *) * (*token_count + 1));
	(*tokens)[*token_count] = strdup(token);
	(*token_count)++;
}