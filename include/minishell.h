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
// #					CUSTOM INCLUDES					  #
// ########################################################

# include "../libft/include/libft.h"
# include "../libft/include/colors.h"
# include "funcs.h"
# include "struct.h"

// ########################################################
// #					OTHERS INCLUDES					  #
// ########################################################

# include "struct.h"
# include "funcs.h"
# include "define.h"

// ########################################################
// #					DEFAULTS INCLUDES				  #
// ########################################################

//   #include <readline/readline.h> BOUH LA HONTE CE TRUC AAAAAAHHHHHHHHHHHHHHHHHHHHHHH
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <fcntl.h>
# include <termios.h>

#endif
