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
	t_history	*tmp;
	t_history	*next;

	tmp = minishell->history;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->cmd);
		free(tmp);
		tmp = next;
	}
	free(minishell);
}

t_minishell	*init_minishell(void)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		return (NULL);
	refresh_history(minishell);
	return (minishell);
}

int	main(int argc, char **args, char **env)
{
	struct termios	original_termios;
	t_termios		termios;
	t_minishell		*minishell;
	char			*input;

	(void)argc;
	(void)args;
	(void)env;
	minishell = init_minishell();
	termios.original_termios = &original_termios;
	minishell->termios = &termios;
	enable_raw_mode(&termios);
	while (1)
	{
		ft_putstr_fd(BOLDWHITE"minishell$ ", 1);
		if (!wait_input(&input, minishell))
		{
			disable_raw_mode(&termios);
			free(input);
			break ;
		}
		ft_putchar_fd('\n', 1);
		erase_term(ft_strlen(input));
		if (exec_command(minishell, input))
			break ;
		free(input);
	}
	disable_raw_mode(&termios);
	reset_history();
	destroy_minishell(minishell);
	return (0);
}
