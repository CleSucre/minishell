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
static char	*get_token_type(int type)
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
 * @return void
 */
void	debug_tokens(t_token *tokens)
{
	t_token	*tmp;
	int		i;
	char	*type;

	if (!DEBUG)
		return ;
	tmp = tokens;
	i = 0;
	while (tmp)
	{
		type = get_token_type(tmp->type);
		ft_printf("\n%s[DEBUG] ====== tokens [%d] ======%s\n", BLUE, i, RESET);
		ft_printf("token: %s%s%s\n", YELLOW, tmp->value, RESET);
		ft_printf("type str: %s%s%s (id: %d)\n",
			BOLDWHITE, type, RESET, tmp->type);
		tmp = tmp->next;
		i++;
	}
}
