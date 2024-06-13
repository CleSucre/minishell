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
 * @brief Get the type of token from string for secondary tokens
 * @param char *str
 * @return t_type
 */
static t_type	token_type_secondary(char *str)
{
	if (ft_strncmp(str, "&&", 2) == 0)
		return (AND_OPERATOR);
	else if (ft_strncmp(str, "||", 2) == 0)
		return (OR_OPERATOR);
	else if (ft_strncmp(str, ";", 1) == 0)
		return (SEMICOLON);
	return (COMMAND);
}

/**
 * @brief Get the type of token from string
 *
 * @param char *str
 * @return t_type
 */
t_type	token_type_primary(char *str)
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
	else if (ft_strncmp(str, "$", 1) == 0 && ft_strlen(str) > 1)
		return (VARIABLE);
	else if (ft_strncmp(str, "\"", 1) == 0)
		return (TEXT_DOUBLE_QUOTE);
	else if (ft_strncmp(str, "\'", 1) == 0)
		return (TEXT_SINGLE_QUOTE);
	return (token_type_secondary(str));
}
