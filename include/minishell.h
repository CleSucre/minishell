/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef DEBUG
#  define DEBUG 0
# endif

# define TERMINAL_PROMPT "minishell$ "

// ########################################################
// #						LIBFT						  #
// ########################################################

# include "../libft/include/colors.h"
# include "../libft/include/libft.h"

// ########################################################
// #						MINISHELL					  #
// ########################################################

# include "define.h"
# include "struct.h"
# include "config.h"
# include "execution.h"
# include "history.h"
# include "memory.h"
# include "terminal.h"
# include "parsing.h"

// ########################################################
// #						DEFAULTS					  #
// ########################################################

//# include <readline/readline.h> BOUH LA HONTE CE TRUC AAAAAAHHHHHHHHHHHHHHHHHHHHHHH !! BURK LES ~400 LEAKS !!!
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <dirent.h>
# include <termios.h>
# include <sys/wait.h>
# include <signal.h>
# include <ncurses.h>
# include <term.h>

#endif
