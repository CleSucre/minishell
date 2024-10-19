/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 04:02:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/19 04:02:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check the following things:
 * 	- If the input is empty, print a new line and return NULL
 * 	- Trim the input
 * 	- Add the input to the history
 * 	- Print the input in debug mode if needed
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return char* the trimmed input
 */
char	*check_input(char *input)
{
	char	*trimmed;

	if (ft_strlen(input) == 0)
	{
		terminal_print("", 1, STDOUT_FILENO);
		free(input);
		return (NULL);
	}
	trimmed = ft_strtrim(input, WHITESPACES);
	free(input);
	if (!trimmed)
		return (NULL);
	debug_execution(trimmed);
	return (trimmed);
}
