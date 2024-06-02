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
	t_minishell		*minishell;
	struct termios	original_termios;
	char			*input;

	minishell = init_minishell();
	enable_raw_mode(&original_termios);
	while (1)
	{
		is_raw(&original_termios);
		input = wait_input(minishell, BOLDWHITE"minishell$");
		if (exec_command(minishell, input))
			break ;
	}
	disable_raw_mode(&original_termios);
	reset_history();
	destroy_minishell(minishell);
	return (0);
}
