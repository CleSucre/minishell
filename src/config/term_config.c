/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Save the original termios settings
 *
 * @param t_term *termios
 * @return void
 */
static void	save_termios(t_term *termios)
{
	if (tcgetattr(STDIN_FILENO, termios->original_termios) == -1)
		perror("tcgetattr");
}

/**
 * @brief Enable termios to read char, and catch exhaust sequence
 *
 * @param t_term *term
 * @return void
 */
void	enable_termios(t_term *term)
{
	struct termios	new_termios;

	save_termios(term);
	new_termios = *term->original_termios;
	new_termios.c_lflag &= ~(ECHO | ICANON | ISIG);
	new_termios.c_cc[VERASE] = 127;
	new_termios.c_iflag &= ~(ICRNL | INLCR | IGNCR);
	new_termios.c_cc[VMIN] = 1;
	new_termios.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1)
		perror("tcsetattr");
}

/**
 * @brief Restore default settings of termios
 *
 * @param t_term *termios
 * @return void
 */
void	disable_termios(t_term *termios)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, termios->original_termios) == -1)
		perror("tcsetattr");
}
