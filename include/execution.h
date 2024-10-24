/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/17 10:15:34 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "struct.h"
# include <errno.h>

# define MAX_HEREDOC 17

// ########################################################
// #						EXECUTOR						#
// ########################################################

int				execute_external(t_minishell *minishell, t_cmd *cmd);
int				execute_cmd(t_minishell *minishell,
					t_ast_node *ast, int pipes[2], int in_out[3]);
int				execute_ast(t_minishell *minishell, t_ast_node *ast, int *pipes,
					int *in_out);
int				execute_input(t_minishell *minishell, char *input);
//						execution_verif
int				pre_executed_failed(t_minishell *minishell, t_ast_node *ast);
void			exceed_max_heredoc(t_minishell *minishell, t_ast_node *ast);

// ########################################################
// #					EXECUTION_OR_AND					#
// ########################################################

int				execute_or(t_minishell *minishell, t_ast_node *ast,
					int *pipes, int *in_out);
int				execute_and(t_minishell *minishell, t_ast_node *ast,
					int *pipes, int *in_out);

// ########################################################
// #					EXECUTE PATH						#
// ########################################################

int				execute_path(t_cmd *cmd);

// ########################################################
// #					SIGNAL HANDLER						#
// ########################################################

//void	handle_signal(int sig);
int				wait_for_processes(void);
int				wait_for_pid(int pid);

// ########################################################
// #						REDIRECTIONS					#
// ########################################################

void			close_fds(int in_out[2], int *fd);
void			close_all_fds(int *to_close);
void			add_fd_to_close(int *to_close, int fd);
int				setup_pipes(int *pipes, int *in_out, int is_last);
int				*init_to_close(void);
ssize_t			copy_fd_contents(int fd_from, int fd_to);
int				execute_pipe(t_minishell *minishell, t_ast_node *ast,
					int *pipes, int *in_out);
int				verify_redirection(t_minishell *minishell, t_ast_node *ast);
int				execute_redirect_input(t_minishell *minishell, t_ast_node *ast,
					int *pipes, int *in_out);
int				heredoc_valid(t_ast_node *ast);
int				pre_execute_heredoc(t_minishell *minishell, t_ast_node *ast);
int				run_heredoc(t_minishell *minishell,
					char *delimiter, int *output_fd);
int				execute_heredoc(t_minishell *minishell, t_ast_node *ast,
					int *pipes, int *in_out);
int				execute_redirect_output(t_minishell *minishell, t_ast_node *ast,
					int *pipes, int *in_out);
int				execute_redirect_output_append(t_minishell *minishell,
					t_ast_node *ast, int *pipes, int *in_out);

// ########################################################
// #						HEREDOC							#
// ########################################################

int				handle_heredoc_parent_process(t_minishell *minishell,
					int *tmp_pipe, pid_t pid, int *output_fd);
void			write_and_cleanup_heredoc(t_heredoc_info *heredoc_info, int fd);
void			heredoc_signal_handler(int sig);
void			parent_signal_handler(int sig);
int				write_heredoc(t_heredoc_info *heredoc_info);
int				read_heredoc(t_heredoc_info *heredoc_info);

#endif
