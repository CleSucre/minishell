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
	free_history(minishell->history);
	free(minishell);
}

t_minishell	*init_minishell(void)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		return (NULL);
	minishell->history = malloc(sizeof(t_history));
	if (!minishell->history)
	{
		free(minishell);
		return (NULL);
	}
	minishell->history->cmd = NULL;
	minishell->history->older = NULL;
	minishell->history->newer = NULL;
	minishell->history->pos = 0;
	load_history(minishell->history);
	return (minishell);
}

int	main(int argc, char **args, char **env)
{
	t_minishell		*minishell;
	struct termios	original_termios;
	t_termios		termios;

	(void)argc;
	(void)args;
	(void)env;
	minishell = init_minishell();
	termios.original_termios = &original_termios;
	enable_raw_mode(&termios);
	use_termios(minishell);
	disable_raw_mode(&termios);
	destroy_minishell(minishell);
	return (0);
}
