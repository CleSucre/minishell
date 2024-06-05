/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parse the input and create ast
 *
 * @param char *input input to parse
 * @return t_ast *
 */
t_ast	*parse_input(t_minishell *minishell, char *input)
{
	t_ast *ast;
	t_token *start;
    t_token *tokens;
	int		i;

	if (!input)
		return (NULL);
	ast = NULL;
	tokens = tokenize(input);
	start = tokens;
    if (!tokens)
        return (NULL);
	print_tokens(tokens);
	i = 0;
	while (tokens)
	{
		tokens = tokens->next;
		i++;
	}
	free_tokens(start);
    return (ast);
}
