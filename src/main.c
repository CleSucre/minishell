/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the shell is running in
 * 			a pipe (stdin or stdout not a terminal)
 *
 * @return int 1 if stdin or stdout is not a terminal, 0 otherwise.
 */
int	is_running_in_pipeline(void)
{
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (1);
	return (0);
}

/**
 * @brief Main function of the minishell
 *
 * @param int argc
 * @param char **args
 * @param char **env
 * @return int
 */
int	main(int argc, char **args, char **env)
{
	t_minishell		*minishell;
	struct termios	original_termios;
	int				exit_code;

	(void)argc;
	(void)args;
	if (is_running_in_pipeline())
	{
		fprintf(stderr, "Error: minishell cannot be run in a pipeline.\n");
		return (1);
	}
	minishell = alloc_minishell();
	minishell->term->original_termios = &original_termios;
	minishell->env = ft_tabdup((const char **)env);
	enable_termios(minishell->term);
	use_termios(minishell);
	disable_termios(minishell->term);
	exit_code = minishell->exit_code;
	free_minishell(minishell);
	ft_printf("exit code: %d\n", exit_code);
	return (exit_code);
}
