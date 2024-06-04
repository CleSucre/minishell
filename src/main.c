/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	destroy_minishell(t_minishell *minishell)
{
	history_free(minishell->history);
	free(minishell);
}

/**
 * @brief Init minishell structure
 * @return
 */

t_minishell	*init_minishell(void)
{
	t_minishell	*minishell;

	minishell = ft_calloc(sizeof(t_minishell), 1);
	if (!minishell)
		return (NULL);
	minishell->history = ft_calloc(sizeof(t_history), 1);
	if (!minishell->history)
	{
		free(minishell);
		return (NULL);
	}
	minishell->exit_code = 0;
	minishell->history->cmd = NULL;
	minishell->history->older = NULL;
	minishell->history->newer = NULL;
	minishell->history->pos = 0;
	history_load(minishell);
	return (minishell);
}

/**
 * @brief 	Create minishell structure to create and fill .ministory
 * 			Enable raw mode to catchs args et sig
 * 			End
 * @param argc
 * @param args
 * @param env
 * @return
 */
int	main(int argc, char **args, char **env)
{
	t_minishell		*minishell;
	struct termios	original_termios;
	t_termios		termios;
	int 			exit_code;

	(void)argc;
	(void)args;
	(void)env;
	minishell = init_minishell();
	termios.original_termios = &original_termios;
	enable_raw_mode(&termios);
	use_termios(minishell);
	disable_raw_mode(&termios);
	exit_code = minishell->exit_code;
	destroy_minishell(minishell);
	return (exit_code);
}
