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

void	add_to_history(t_minishell *minishell, char *cmd);
int		refresh_history(t_minishell *minishell);
void	reset_history(void);
int		get_history_file(void);

// ########################################################
// #						EXECOTOR					  #
// ########################################################

char	*wait_input(t_minishell *minishell, const char *prompt);
int		exec_command(t_minishell *minishell, char *input);

// ########################################################
// #						SIG_HANDLER					  #
// ########################################################

void	sig_handler(int sig, siginfo_t *info, void *context);
void	setup(void);

// ########################################################
// #						RAW 						  #
// ########################################################

void	disable_raw_mode(const struct termios *original_termios);
void	enable_raw_mode(struct termios *original_termios);
int		is_raw(struct termios *original_termios);

// ########################################################
// #						MAIN						  #
// ########################################################

// ########################################################
// #						PARSING						  #
// ########################################################

#endif