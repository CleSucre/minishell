/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get token type as string
 *
 * @param int type token type
 * @return char * token type as string
 */
char	*get_token_type(int type)
{
	if (type == PIPE)
		return ("PIPE");
	else if (type == REDIRECT_IN)
		return ("REDIRECT_IN");
	else if (type == REDIRECT_OUT)
		return ("REDIRECT_OUT");
	else if (type == REDIRECT_APPEND)
		return ("REDIRECT_APPEND");
	else if (type == COMMAND)
		return ("COMMAND");
	else if (type == ARGUMENT)
		return ("ARGUMENT");
	else if (type == QUOTE)
		return ("QUOTE");
	else if (type == VARIABLE)
		return ("VARIABLE");
	else
		return ("UNKNOWN");
}


/**
 * @brief Print tokens list
 *
 * @param t_token *tokens list of tokens
 * @return None
 */
void	print_tokens(t_token *tokens)
{
	t_token	*tmp;
	int			i;
	char		*type;

	tmp = tokens;
	i = 0;
	while (tmp)
	{
		type = get_token_type(tmp->type);
		ft_printf("\n====== tokens [%d] ======\ntoken: %s\ntype str: %s (id: %d)",
				  i, tmp->value, type, tmp->type);
		tmp = tmp->next;
		i++;
	}
}
