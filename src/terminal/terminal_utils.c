/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 03:41:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/06/14 03:41:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Clear the lines and put back prompt after moving cursor
 *
 * @param cols
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
 * @param char * String to print
 * @param int nl Move cursor down and print newline(s)
 * @return void
 */
void	terminal_print(char *str, int nl, int fd)
{
	if (nl)
		ft_fprintf(fd, "\033[%dD", 100);
	while (nl--)
		ft_putchar_fd('\n', fd);
	ft_fprintf(fd, "%s", str);
}

/**
 * @brief Reset input string
 *
 * @param char **input
 * @return void
 */
void	reset_input(char **input)
{
	if (*input)
	{
		free(*input);
		*input = NULL;
	}
	*input = ft_calloc(1, sizeof(char *));
}
