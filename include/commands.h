/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 07:13:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 07:13:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_COMMANDS_H
# define MINISHELL_COMMANDS_H

# include "parsing.h"
# include "struct.h"

typedef struct s_cmd
{
	char			*cmd_name;
	char			*cmd_exec;
	struct	s_cmd	*prev;
	struct	s_cmd	*next;
	int				argc;
	char			**argv;
	char			**env;
	char			*path;
	int             input;
	int             output;
    int             fd_to_close;
	int				exit_status;
}	t_cmd;

// ########################################################
// #						COMMANDS					  #
// ########################################################

int		command_cd(t_cmd *cmd);
int		command_echo(t_cmd *cmd);
void	command_env(t_cmd *cmd);
void	command_exit(t_cmd *cmd);
void	command_export(t_cmd *cmd);
void	command_history(t_cmd *cmd, t_minishell *minishell);
void	command_pwd(t_cmd *cmd);
void	command_unset(t_cmd *cmd);

// ########################################################
// #						UTILS						  #
// ########################################################

t_cmd	*load_command(t_minishell *minishell, t_ast *cmd, int pipe_fd[2], int fd_to_close);

// ########################################################
// #						FLAGS_UTILS					  #
// ########################################################

int		contain_flag(char **argv, char flag);

// ########################################################
// #						COMMAND_MANAGER				  #
// ########################################################

int		execute_custom_command(t_minishell *minishell, t_cmd *cmd);


#endif
