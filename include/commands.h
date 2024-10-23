/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:19:46 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/17 10:19:46 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_H
# define COMMANDS_H

# include "parsing.h"
# include "struct.h"

typedef struct s_cmd
{
	char	*name;
	char	**args;
	int		argc;
	int		input_fd;
	int		output_fd;
	char	**env;
	char	*path;
	int		to_close;
	int		exit_signal;
}				t_cmd;

// ########################################################
// #						COMMANDS						#
// ########################################################

int			command_cd(t_minishell *minishell, t_cmd *cmd);
int			command_echo(t_cmd *cmd);
int			command_env(t_cmd *cmd);
int			command_exit(t_cmd *cmd);
int			command_export(t_minishell *minishell, t_cmd *cmd);
int			command_history(t_cmd *cmd, t_minishell *minishell);
int			command_pwd(t_cmd *cmd);
int			command_unset(t_cmd *cmd);

// ########################################################
// #						UTILS							#
// ########################################################

void		destroy_cmd(t_cmd *cmd);
t_cmd		*create_cmd(t_ast_node *ast, t_minishell *minishell,
				const int in_out[3]);	
// Export
int			print_export(t_cmd *cmd);
int			sum_cmd_env(t_cmd *cmd, char *input, char *value);
int			add_cmd_env(t_minishell *minishell, char *input, char *value);
// Cd
int			cd_minus(t_cmd *cmd);
void		invert_oldpwd(t_cmd *cmd);
int			check_oldpwd(t_minishell *minishell, t_cmd *cmd);
int			ft_check_access(const char *name);
long long	ft_strtoll(const char *str, char **endptr, int base);
// Exit
char		*noptozero(char *tmp);

// ########################################################
// #						FLAGS_UTILS						#
// ########################################################

int			contain_flag(char **argv, char flag);

// ########################################################
// #						COMMAND_MANAGER					#
// ########################################################

int			execute_builtin_command(t_minishell *minishell, t_cmd *cmd);
int			is_builtin_command(t_cmd *cmd);

#endif
