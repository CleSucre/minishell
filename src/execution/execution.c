/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return int 1 on success, 0 on failure
 */
int	exec_command(t_minishell *minishell, char *input)
{
	t_ast	*ast;
	int		res;

	if (ft_strlen(input) == 0)
		return (0);
	input = ft_strtrim(input, WHITESPACES);
	if (!input)
		return (0);
	if (DEBUG)
	{
		terminal_print(BOLDWHITE"[DEBUG] "RESET"Command "BOLDWHITE, 1);
		terminal_print(input, 0);
		terminal_print(RESET" executed", 0);
	}
	if (ft_isprint(*input))
		history_add(minishell, input, 1);
	res = 0;
	if (ft_strncmp(input, "exit", 4) == 0)
		res = 1;
	else if (ft_strcmp(input, "history") == 0)
		history_print(minishell);
	else
	{
		ast = parse_input(minishell, input);
		if (!ast)
		{
			free(input);
			return (0);
		}
		//execute_ast(minishell, ast);
		free_ast(ast);
	}
	free(input);
	return (res);
}
