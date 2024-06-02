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
#define FILE "raw.c"

/**
 * Termios struct is used to set shell's settings in UNIX and Linux systems
 * It's used to control differents communications options and signals send to
 * the terminal, as canonical mode, echo and transmission speed
 */

/**
 * @brief enable raw mode to read char, and catch exhaust sequence
 * @param original_termios
 * @return none
 */

void enable_raw_mode(t_termios *termios) {
    struct termios raw;

    // Obtenir les attributs du terminal actuel
    if (tcgetattr(STDIN_FILENO, termios->original_termios) == -1) {
        perror("tcgetattr");
        return;
    }

    // Copier les attributs pour les modifier
    raw = *termios->original_termios;

    // Modifier les attributs pour le mode brut
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON | ICRNL);
    raw.c_cflag |= (CS8);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    // Appliquer les nouveaux attributs du terminal
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        return;
    }
}

/**
 * @brief Restore default settings
 * @param original_termios
 * @return none
 */

void disable_raw_mode(t_termios *termios) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, termios->original_termios) == -1) {
        perror("tcsetattr");
		return ;
    }
}
