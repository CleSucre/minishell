/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 07:37:48 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "struct.h"
# include <errno.h>

// ########################################################
// #						EXECUTOR					  #
// ########################################################

int		execute_cmd(t_minishell *minishell, t_ast_node *ast, int pipes[2], int in_out[3]);
int		execute_ast(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out);
int		execute_input(t_minishell *minishell, char *input);

// ########################################################
// #					EXECUTE PATH					  #
// ########################################################

int		execute_path(t_cmd *cmd);

// ########################################################
// #					SIGNAL HANDLER					  #
// ########################################################

void	handle_signal(int sig);
int		wait_for_processes(void);

// ########################################################
// #						REDIRECTIONS				  #
// ########################################################

void	close_fds(int in_out[2], int *fd);
int		setup_pipes(int *pipes, int *in_out, int is_last);
ssize_t	copy_fd_contents(int fd_from, int fd_to);
int		execute_pipe(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out);
int		execute_redirect_input(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out);
int		execute_heredoc(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out);
int		execute_redirect_output(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out, int append);

#endif
