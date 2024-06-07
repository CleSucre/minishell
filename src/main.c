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
 * @brief	Main function of the minishell
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
	(void)env;
	minishell = alloc_minishell();
	minishell->term->original_termios = &original_termios;
	enable_termios(minishell->term);
	use_termios(minishell);
	disable_termios(minishell->term);
	exit_code = minishell->exit_code;
	free_minishell(minishell);
	return (exit_code);
}
