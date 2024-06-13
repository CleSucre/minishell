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
 * @brief Execute custom command before parsing the input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return int 1 if a command was executed, 0 otherwise,
 * 			2 if exit was called
 */
static int	execute_custom_command(t_minishell *minishell, char *input)
{
	if (ft_strncmp(input, "exit", 4) == 0)
		return (-1);
	else if (ft_strcmp(input, "history") == 0)
	{
		history_print(minishell);
		return (1);
	}
	return (0);
}

/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return int 1 on success, 0 on failure
 */
int	execute_command(t_minishell *minishell, char *input)
{
	t_ast	*ast;
	int		res;

	if (ft_strlen(input) == 0)
		return (0);
	input = ft_strtrim(input, WHITESPACES);
	if (!input)
		return (0);
	debug_execution(input);
	if (ft_isprint(*input))
		history_add(minishell, input, 1);
	res = execute_custom_command(minishell, input);
	if (res == -1)
		return (1);
	else if (res == 1)
		return (0);
	ast = parse_input(minishell, input);
	if (!ast)
	{
		free(input);
		return (0);
	}
	free_ast(ast);
	free(input);
	return (0);
}
