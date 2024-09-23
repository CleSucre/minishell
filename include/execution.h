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

int		execute_cmd(t_minishell *minishell, t_ast_node *ast, int input_fd, int output_fd);
int		execute_input(t_minishell *minishell, char *input);

int		execute_path(t_cmd *cmd);

#endif
