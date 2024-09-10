/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 09:53:22 by julthoma         ###   ########.fr       */
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
static char	*check_input(t_minishell *minishell, char *input)
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
	if (ft_isprint(*trimmed))
		history_add(minishell, trimmed, 1);
	return (trimmed);
}

/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return 0 on success -1 on exit request
 */
int	execute(t_minishell *minishell, char *input)
{
	t_ast	*ast;
	int		res;
	char	*trimmed;

	trimmed = check_input(minishell, input);
	if (!trimmed)
		return (0);
	ast = parse_input(minishell, trimmed);
	if (!ast)
	{
		free(trimmed);
		return (0);
	}
	free(trimmed);
	res = execute_cmds(minishell, ast);
	free_ast(ast);
	minishell->exit_code = res;
	return (res);
}
