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

# include <stdio.h>
//   #include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <fcntl.h>
# include <termios.h>

#endif
