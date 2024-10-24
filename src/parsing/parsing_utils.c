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

/**
 * @brief Check if simple and double quotes are correctly closed
 * 			in respectives order.
 * 			If a ' is between two ", it is not counted.
 * 			Same for ".
 *
 * @param char *str
 * @return int 1 if the quotes are correctly closed, 0 otherwise
 */
int	check_quotes_count(char *str)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && double_quote == 0)
			single_quote = !single_quote;
		else if (str[i] == '"' && single_quote == 0)
			double_quote = !double_quote;
		i++;
	}
	if (single_quote || double_quote)
		return (0);
	return (1);
}
