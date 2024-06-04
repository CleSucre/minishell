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
#include <termios.h>

/**
 * Termios struct is used to set shell's settings in UNIX and Linux systems
 * It's used to control differents communications options and signals send to
 * the terminal, as canonical mode, echo and transmission speed
 */
void	save_termios(t_termios *termios)
{
	if (tcgetattr(STDIN_FILENO, termios->original_termios) == -1)
		perror("tcgetattr");
}

/**
 * @brief enable raw mode to read char, and catch exhaust sequence
 * @param original_termios
 * @return none
 */
void	enable_raw_mode(t_termios *termios)
{
	struct termios	new_termios;

	save_termios(termios);
	new_termios = *termios->original_termios;
	new_termios.c_lflag &= ~(ECHO | ICANON | ISIG);
	new_termios.c_cc[VERASE] = 127;
	new_termios.c_iflag &= ~(ICRNL | INLCR | IGNCR);
	new_termios.c_cc[VMIN] = 1;
	new_termios.c_cc[VTIME] = 0;

	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1)
		perror("tcsetattr");
}

/**
 * @brief Restore default settings
 * @param original_termios
 * @return none
 */
void	disable_raw_mode(t_termios *termios)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, termios->original_termios) == -1)
		perror("tcsetattr");
}
