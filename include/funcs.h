/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funcs.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCS_H
# define FUNCS_H

# include "struct.h"
# include <signal.h>

// ########################################################
// #						HISTORY						  #
// ########################################################

t_history	*history_up(t_minishell *minishell);
t_history	*history_down(t_minishell *minishell);
t_history	*history_get_current(t_minishell *minishell);
t_history	*history_find(t_minishell *minishell, char *cmd, int direction);
int			history_add(t_minishell *minishell, char *cmd, int fs);
int			history_load(t_minishell *minishell);
void		history_free(t_history *history);
void		history_reset(void);
int			history_get_file(void);

// ########################################################
// #						EXECUTOR					  #
// ########################################################

int			use_termios(t_minishell *minishell);
int			exec_command(t_minishell *minishell, char *input);

// ########################################################
// #						RAW 						  #
// ########################################################

void		disable_raw_mode(t_termios *termios);
void		enable_raw_mode(t_termios *termios);
void		process_raw_arrow(t_termios *termios);

// ########################################################
// #						TERMINAL					  #
// ########################################################

void		erase_term(size_t len);
void		terminal_print(char *str, int nl);

// ########################################################
// #						MAIN						  #
// ########################################################

// ########################################################
// #						PARSING						  #
// ########################################################

#endif