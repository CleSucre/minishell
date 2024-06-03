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

char	*search_history(t_minishell *minishell, char *cmd);
void	add_to_history(t_minishell *minishell, char *cmd);
int		refresh_history(t_minishell *minishell);
void	reset_history(void);
int		get_history_file(void);

// ########################################################
// #						EXECOTOR					  #
// ########################################################

int		use_termios(t_minishell *minishell);
int		exec_command(t_minishell *minishell, char *input);

// ########################################################
// #						RAW 						  #
// ########################################################

void	disable_raw_mode(t_termios *termios);
void	enable_raw_mode(t_termios *termios);
void	process_raw_arrow(t_termios *termios);

// ########################################################
// #						TERMINAL					  #
// ########################################################

void	erase_term(size_t len);
void	terminal_print(char *str, int nl);

// ########################################################
// #						MAIN						  #
// ########################################################

// ########################################################
// #						PARSING						  #
// ########################################################

#endif