/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raw.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define FILE "raw.c"

/**
 * Termios struct is used to set shell's settings in UNIX and Linux systems
 * It's used to control differents communications options and signals send to
 * the terminal, as canonical mode, echo and transmission speed
 */

int	is_raw(struct termios *original_termios)
{
	char			c;
	char			seq[3];

	if (read(STDIN_FILENO, &c, 1) == -1)
	{
		perror("read");
		return (1);
	}
	if (c == '\x1b')
	{
		if (read(STDIN_FILEN, &seq[0], 1) == 0
			|| read(STDIN_FILEN, &seq[1], 1) == 0)
			return (1);
		if (seq[0] == '[')
		{
			if (seq[1] == 'A')
				ft_fprintf(1, "fleche haut");
			else if (seq[1] == 'B')
				ft_fprintf(1, "fleche bas");
		}
		else
			ft_fprintf(1, "Caractere lu [%c]", c);
	}
	return (1);
}

/**
 * @brief enable raw mode to read char, and catch exhaust sequence
 * @param original_termios
 * @return none
 */

void	enable_raw_mode(struct termios *original_termios)
{
	stryct termios	raw;

	if (tcgetattr(STDIN_FILENO, original_termios) == -1)
	{
		perror("tcgetattr");
		if (DEBUG)
			ft_fprintf(2, "ERROR IN %s", FILE);
		exit(EXIT_FAILURE);
	}
	raw = *original_termios;
	raw.c_lflag &= ~(ICANON | ECHO);
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
	{
		perror("tcsetattr");
		if (DEBUG)
			ft_fprintf(2, "ERROR IN %s", FILE);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Restore default settings
 * @param original_termios
 * @return none
 */

void	disable_raw_mode(const struct termios *original_termios)
{
	if (tcsetattr(STDILENO, TCSAFLUSH, original_termios) == -1)
	{
		perror("tcsetattr");
		if (DEBUG)
			ft_fprintf(2, "ERROR IN %s", FILE);
		exit(EXIT_FAILURE);
	}
}
