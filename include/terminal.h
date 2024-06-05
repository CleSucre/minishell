/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_H
#define TERMINAL_H

// ########################################################
// #						TERMINAL					  #
// ########################################################

int			interpret_escape_sequence(t_minishell *minishell, char **input, size_t cols);
char		*put_in_string(char *input, char c, size_t cols);
char		*erase_in_string(char *input, size_t cols);
int			use_termios(t_minishell *minishell);

// ########################################################
// #					TERMINAL-ACTION					  #
// ########################################################

void		terminal_print(char *str, int nl);
void		reset_input(char **input);
void		move_cursor_back(size_t position);
int			process_action(t_minishell *minishell, char c, char **input);

// ########################################################
// #				TERMINAL-INFORMATION				  #
// ########################################################

void		get_terminal_size(t_term *term);
void		get_cursor_position(t_term *term);
void		term_info(t_minishell *minishell);
void		get_cursor_position(t_term *term);
void		move_cursor(t_term *term, int x, int y);
void		refresh_cline(t_term *term);
void        del(t_minishell *minishell, char **input);

// ########################################################
// #					GETENT_TEST						  #
// ########################################################

int		my_tgetent(t_term *info, const char *term_type);
char *ft_strncpy(char *dest, const char *src, size_t n);


#endif
