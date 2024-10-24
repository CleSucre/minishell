/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/17 10:19:10 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// ########################################################
// #						LIBFT							#
// ########################################################

# include "../libft/include/colors.h"
# include "../libft/include/libft.h"

// ########################################################
// #						MINISHELL						#
// ########################################################

# include "commands.h"
# include "completion.h"
# include "config.h"
# include "environment.h"
# include "execution.h"
# include "history.h"
# include "memory.h"
# include "parsing.h"
# include "struct.h"
# include "terminal.h"
# include "wildcard.h"

// ########################################################
// #						DEFAULTS						#
// ########################################################

# include <fcntl.h>
# include <ncurses.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

#endif
