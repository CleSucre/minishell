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
	return (token);
}

/**
 * @brief Add a token to the token list
 *
 * @param head The head of the token list
 * @param new The new token to add
 */
void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/**
 * @brief Check if the token corresponds to type variable given
 *
 * @param t_token		*token
 * @param t_token_type	*type
 * @return int 1 if the next token is a command, 0 otherwise
 */
int is_token(t_token *token, t_token_type type)
{
	if (!token)
		return (0);
	if (token->type == type)
		return (1);
	return (0);
}