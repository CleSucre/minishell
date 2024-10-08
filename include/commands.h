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

typedef struct s_cmd {
	char	*name;			// Command name (e.g., "ls", "echo")
	char	**args;			// Command arguments (e.g., ["ls", "-la", NULL])
	int 	argc;			// Number of arguments
	int		input_fd;		// Input file descriptor (default: STDIN_FILENO)
	int		output_fd;		// Output file descriptor (default: STDOUT_FILENO)
	char	**env;			// Environment variables (e.g., ["PATH=/usr/bin", ...])
	char	*path;			// Full path of the executable (e.g., "/bin/ls")
	int		exit_code;		// Exit code of the command
} t_cmd;


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

void	destroy_cmd(t_cmd *cmd);
t_cmd	*create_cmd(t_ast_node *ast, char **envp, int input_fd, int output_fd);

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
