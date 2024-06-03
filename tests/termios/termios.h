//
// Created by Julie on 02/06/2024.
//

#ifndef TERMIOS_H
# define TERMIOS_H

# ifndef DEBUG
#  define DEBUG 0
# endif

// ########################################################
// #                    CUSTOM INCLUDES                   #
// ########################################################

# include "../../../libft/include/libft.h"
# include "../../../libft/include/colors.h"

// ########################################################
// #                    DEFAULTS INCLUDES                 #
// ########################################################

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include "termios.h"
# include <fcntl.h>

typedef struct s_termios
{
	struct termios	*original_termios;
}		t_termios;

void init_termios(t_termios *term);
void set_non_canonical_mode(t_termios *term);
void restore_termios(t_termios *term);

#endif
