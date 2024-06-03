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

void save_termios(t_termios *termios) {
	if (tcgetattr(STDIN_FILENO, termios->original_termios) == -1) {
		perror("tcgetattr");
	}
}

/**
 * @brief enable raw mode to read char, and catch exhaust sequence
 * @param original_termios
 * @return none
 */
void enable_raw_mode(t_termios *termios) {
    struct termios new_termios;

	new_termios = *termios->original_termios;

	save_termios(termios);

	// Désactiver les signaux de contrôle, l'écho, le mode canonique, etc.
	// ECHO : Echo des caractères saisis
	// ICANON : Mode canonique, si activé, les caractères sont lus ligne par ligne
	// IEXTEN : Caractères d'extension (Ctrl+V, Ctrl+O)
	// ISIG : Signaux de contrôle (Ctrl+C, Ctrl+Z)
	new_termios.c_lflag &= ~(ECHO | ICANON | ISIG);

	new_termios.c_cc[VERASE] = 127; //127 est le code ASCII pour DEL


	// Désactiver le contrôle de flux sauf ICRNL pour activer la conversion CR à NL
	// BRKINT : Si défini, un signal d'interruption est envoyé à l'application et non au processus
	// INPCK : Vérification de parité
	// ISTRIP : Dépouillement des bits
	// IXON : Contrôle de flux logiciel
	// ICRNL : Conversion CR à NL
	// INLCR : Conversion NL à CR
	// IGNCR : Ignorer CR
	new_termios.c_iflag &= ~(ICRNL | INLCR | IGNCR);

	// Désactiver le contrôle de sortie
	// CS8 : Configurer les caractères à 8 bits
	// HUPCL : Fermeture du terminal lors de la dernière sortie
	//new_termios.c_cflag |= (CS8 | HUPCL);

    //new_termios.c_oflag &= ~(OPOST);

	//VMIN : Nombre minimal de caractères pour lire (en mode non canonique).
    new_termios.c_cc[VMIN] = 1;
    //VTIME : Temps d'attente pour lire (en mode non canonique).
	new_termios.c_cc[VTIME] = 1; //1/10ème de seconde

   // Appliquer les nouveaux paramètres immédiatement
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1) {
        perror("tcsetattr");
    }
}

/**
 * @brief Restore default settings
 * @param original_termios
 * @return none
 */

void disable_raw_mode(t_termios *termios)
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, termios->original_termios) == -1)
        perror("tcsetattr");
}
