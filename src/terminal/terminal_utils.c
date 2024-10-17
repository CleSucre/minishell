/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 03:41:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/17 02:30:49 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Clear the lines and put back prompt after moving cursor
 *
 * @param t_minishell *minishell
 */
void	reset_stdin(t_minishell *minishell)
{
	ft_putstr_fd("\033[2K", 1);
	move_cursor_back(minishell->term->cols);
	print_terminal_prompt(minishell, 0);
}

/**
 * @brief Print in our terminal
 * 			if nl, move cursor one line down
 *
 * @param char *str String to print
 * @param int nl Move cursor down and print newline(s)
 * @param int fd File descriptor
 */
void	terminal_print(char *str, int nl, int fd)
{
	if (nl && fd == STDOUT_FILENO)
		ft_putstr_fd("\033[100D", fd);
	while (nl--)
		ft_putchar_fd('\n', fd);
	ft_putstr_fd(str, fd);
}

/**
 * @brief Reset input string and realloc it
 *
 * @param char ***input
 */
void	reset_input(char ***input)
{
	if (*input)
		ft_tabfree(*input);
	*input = ft_calloc(1, sizeof(char *));
}

/**
 * @brief Set tabstop every 4, will possibly be deleted
 *
 * @param t_minishell *minishell
 */
void	set_tabstop(t_minishell *minishell)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	ft_putstr_fd("\033[s", 1);
	ft_putstr_fd("\033[3g", 1);
	while (j < minishell->term->ws_rows)
	{
		while (i < minishell->term->ws_cols)
		{
			i += 4;
			ft_putstr_fd("\033[4C", 1);
			ft_putstr_fd("\033H", 1);
		}
		ft_putstr_fd("\033[B", 1);
		j++;
		i = 0;
	}
	ft_putstr_fd("\033[u", 1);
}
