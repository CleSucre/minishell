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

void	handle_signal(int sig);
int		wait_for_processes(int *status);
int		execute_path(t_cmd *cmd);
int		execute_cmds(t_minishell *minishell, t_ast_node *ast);
int		execute_cmd(t_minishell *minishell, t_ast_node *ast, int in_out[2], int to_close);
int		execute(t_minishell *minishell, char *input);

#endif
