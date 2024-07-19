/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 02:34:45 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_H
#define TERMINAL_H

# define TERMINAL_EXIT_MSG BOLDYELLOW "Goodbye !" RESET

// ########################################################
// #					CURSOR MOVES					  #
// ########################################################

# define CURSOR_UP "\033[%dA"
# define CURSOR_DOWN "\033[%dB"
# define CURSOR_LEFT "\033[%dD"
# define CURSOR_RIGHT "\033[%dC"
# define CURSOR_NLINE "\033[E"
# define CURSOR_PLINE "\033[F]"
# define CURSOR_ERA "\033[2K"

// ########################################################
// #					    KEYS						  #
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
// #						TERMINAL					  #
// ########################################################

int				interpret_escape_sequence(t_minishell *minishell, const char *seq, char ***input, size_t cols);
int				use_termios(t_minishell *minishell);

// ########################################################
// #					TERMINAL-ACTION					  #
// ########################################################

void			set_tabstop(t_minishell *minishell);
void			reset_input(char ***input);
void			move_cursor_back(size_t position);
int				process_signals(t_minishell *minishell, char c, char ***input);
int				process_action(t_minishell *minishell, char *new, char ***input);

// ########################################################
// #				TERMINAL-INFORMATION				  #
// ########################################################

void			get_terminal_size(t_term *term);
void			get_cursor_position(t_term *term);
void			term_info(t_term *term);
void			get_cursor_position(t_term *term);
void			move_cursor(t_term *term, int x, int y);

// ########################################################
// #					TERMINAL-UTILS					  #
// ########################################################

void			terminal_print(char *str, int nl, int fd);
void			reset_stdin(t_minishell *minishell);

// ########################################################
// #					TERMINAL-CURSOR					  #
// ########################################################

void			move_cursor_forward(size_t position);
void			move_cursor_back(size_t position);

// ########################################################
// #						ERASE_PUT					  #
// ########################################################

void			put_in_string(t_minishell *minishell, char ***input, char *new);
void			erase_in_string(t_minishell *minishell, char ***input);
void			erase_term(size_t len);

// ########################################################
// #					TERMINAL_ARROW					  #
// ########################################################

void			arrow_up_action(t_minishell *minishell,
					char ***input, t_history *new_history);
void			arrow_down_action(t_minishell *minishell,
					char ***input, t_history *new_history);
void			arrow_left_action(t_minishell *minishell);
void			arrow_right_action(t_minishell *minishell);

// ########################################################
// #					TERMINAL_VARIABLE				  #
// ########################################################

char				*get_var_value(char **env, char *var);
const char			*get_var_value_const(char **env, char *var);
int					get_var_len(char **env, char *var);

// ########################################################
// #					TERMINAL_PROMPT					  #
// ########################################################

unsigned int		get_prompt_len(t_minishell *minishell);
void				print_terminal_prompt(t_minishell *minishell, int new_line);

#endif
