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
 * @brief Add a token to the token list
 *
 * @param tokens The head of the token list
 * @param type The type of the token
 * @param value The value of the token
 */
void	add_token_to_list(t_token **tokens, t_token_type type, char *value)
{
	t_token	*temp;
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->type = type;
	new_token->value = strdup(value);
	new_token->next = NULL;
	new_token->prev = NULL;
	if (!*tokens)
		*tokens = new_token;
	else
	{
		temp = *tokens;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
		new_token->prev = temp;
	}
}

/**
 * @brief Add a token to the token list
 *
 * @param head The head of the token list
 * @param new The new token to add
 */
void	add_token(char ***tokens, int *token_count, char *token)
{
	char	**new_tokens;
	int		i;

	new_tokens = (char **)malloc(sizeof(char *) * (*token_count + 1));
	if (!new_tokens)
		return ;
	i = 0;
	while (i < *token_count)
	{
		new_tokens[i] = (*tokens)[i];
		i++;
	}
	new_tokens[i] = ft_strdup(token);
	if (*tokens)
		free(*tokens);
	*tokens = new_tokens;
	(*token_count)++;
}

void	clear_token_list( t_token **token_list,
							char **tokens, int token_count)
{
	int	i;

	*token_list = NULL;
	i = 0;
	while (i < token_count)
		free(tokens[i++]);
	free(tokens);
	return ;
}
