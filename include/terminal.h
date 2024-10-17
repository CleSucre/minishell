/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:26:45 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/17 10:26:45 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_H
# define TERMINAL_H

# ifndef TERMINAL_EXIT_MSG
#  define TERMINAL_EXIT_MSG "\033[1m\033[33mGoodbye !\033[0m"
# endif

// ########################################################
// #					CURSOR MOVES						#
// ########################################################

# define CURSOR_UP "\033[%dA"
# define CURSOR_DOWN "\033[%dB"
# define CURSOR_LEFT "\033[%dD"
# define CURSOR_RIGHT "\033[%dC"
# define CURSOR_NLINE "\033[E"
# define CURSOR_PLINE "\033[F]"
# define CURSOR_ERA "\033[2K"

// ########################################################
// #							KEYS						  #
// ########################################################

# define ESC_SEQ '\033'
# define BACKSPACE 127
# define DELETE 126
# define CARRIAGE_RETURN 13
# define NEW_LINE 10
# define ENTER 10
# define CTRL_D 4
# define CTRL_C 3
# define L_ARROW 'D'
# define R_ARROW 'C'
# define U_ARROW 'A'
# define D_ARROW 'B'

// ########################################################
// #						TERMINAL						#
// ########################################################

int				interpret_escape_sequence(t_minishell *minishell,
					const char *seq);
void			use_termios(t_minishell *minishell);

// ########################################################
// #					TERMINAL-ACTION						#
// ########################################################

void			set_tabstop(t_minishell *minishell);
void			reset_input(char ***input);
int				process_signals(t_minishell *minishell, char c);
int				process_action(t_minishell *minishell, char *new);

// ########################################################
// #				TERMINAL-INFORMATION					#
// ########################################################

void			get_terminal_size(t_term *term);
void			get_cursor_position(t_term *term);
void			term_info(t_term *term);
void			move_cursor(t_term *term, int x, int y);

// ########################################################
// #					TERMINAL-UTILS						#
// ########################################################

void			terminal_print(char *str, int nl, int fd);
void			reset_stdin(t_minishell *minishell);

// ########################################################
// #					TERMINAL-CURSOR						#
// ########################################################

void			move_cursor_back(size_t n);
void			move_cursor_forward(size_t n);
void			move_cursor_up(size_t n);
void			move_cursor_down(size_t n);

// ########################################################
// #						ERASE_PUT						#
// ########################################################

void			put_in_string(t_minishell *minishell, char *new);
void			erase_in_string(t_minishell *minishell);
void			edit_input(t_minishell *minishell, char *new);
void			erase_term(size_t len);

// ########################################################
// #					TERMINAL_ARROW						#
// ########################################################

void			arrow_up_action(t_minishell *minishell,
					t_history *new_history);
void			arrow_down_action(t_minishell *minishell,
					t_history *new_history);
void			arrow_left_action(t_minishell *minishell);
void			arrow_right_action(t_minishell *minishell);

// ########################################################
// #					TERMINAL_PROMPT						#
// ########################################################

unsigned int	get_prompt_len(t_minishell *minishell);
void			print_terminal_prompt(t_minishell *minishell, int new_line);

#endif
