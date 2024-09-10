/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 07:13:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 09:44:33 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_COMMANDS_H
# define MINISHELL_COMMANDS_H

# include "parsing.h"
# include "struct.h"

typedef struct s_cmd
{
	char			*cmd_name;
	int				argc;
	char			**argv;
	char			**env;
	char			*path;
	int 			input;
	int 			output;
	int 			to_close;
	int				exit_status;
	int 			pid;
}	t_cmd;

// ########################################################
// #						COMMANDS					  #
// ########################################################

int		command_cd(t_cmd *cmd);
int		command_echo(t_cmd *cmd);
void	command_env(t_cmd *cmd);
void	command_exit(t_cmd *cmd);
void	command_export(t_cmd *cmd, t_minishell *minishell);
void	command_history(t_cmd *cmd, t_minishell *minishell);
void	command_pwd(t_cmd *cmd);
void	command_unset(t_cmd *cmd, t_minishell *minishell);

// ########################################################
// #						UTILS						  #
// ########################################################

t_cmd	*load_command(t_minishell *minishell, t_ast *cmd, const int in_out[2], int to_close);

// ########################################################
// #						FLAGS_UTILS					  #
// ########################################################

int		contain_flag(char **argv, char flag);

// ########################################################
// #						COMMAND_MANAGER				  #
// ########################################################

int		execute_builtin_command(t_minishell *minishell, t_cmd *cmd);
int		is_builtin_command(t_cmd *cmd);


#endif
