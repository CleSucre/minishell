/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/14 19:35:36 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief function to invert olwdpwd and pwd when cd - is used
 * @param t_cmd *cmd
 * @param search
 */
static void	invert_oldpwd(t_cmd *cmd)
{
	int		pwd;
	char	*name;
	char	*buff_name;
	int		oldpwd;
	char	buffer[BUFFER_SIZE];

	oldpwd = find_table_args(cmd->env, "OLDPWD");
	pwd = find_table_args(cmd->env, "PWD");
	if (oldpwd == -1 || pwd == -1)
		return ;
	name = strcpy_passing_char(cmd->env[oldpwd], 7);
	if (chdir(name) == -1)
		ft_putstr_fd("Error accessing oldpwd\n", 2);
	free(name);
	clear_string(cmd->env[oldpwd]);
	buff_name = ft_strjoin("OLD", cmd->env[pwd]);
	ft_strlcpy(cmd->env[oldpwd], buff_name,
		(int)ft_strlen(cmd->env[pwd]) + 4);
	free(buff_name);
	clear_string(cmd->env[pwd]);
	name = getcwd(buffer, BUFFER_SIZE);
	buff_name = ft_strjoin("PWD=", name);
	ft_strlcpy(cmd->env[pwd], buff_name, (int)ft_strlen(name) + 5);
	free(buff_name);
}

static void	change_oldpwd(t_cmd *cmd)
{
	int		pwd;
	char	*name;
	char	*buff_name;
	int		oldpwd;
	char	buffer[BUFFER_SIZE];

	oldpwd = find_table_args(cmd->env, "OLDPWD");
	pwd = find_table_args(cmd->env, "PWD");
	if (oldpwd == -1 || pwd == -1)
		return ;
	clear_string(cmd->env[oldpwd]);
	buff_name = ft_strjoin("OLD", cmd->env[pwd]);
	ft_strlcpy(cmd->env[oldpwd], buff_name,
		(int)ft_strlen(cmd->env[pwd]) + 4);
	free(buff_name);
	clear_string(cmd->env[pwd]);
	name = getcwd(buffer, BUFFER_SIZE);
	buff_name = ft_strjoin("PWD=", name);
	ft_strlcpy(cmd->env[pwd], buff_name, (int)ft_strlen(name) + 5);
	free(buff_name);
}

/**
 * @brief Change the current working directory
 * @param cmd The command to execute.
 * By default, as if -L is used
 * @return int 0 if the command was executed successfully, 1 otherwise.
 */
int	command_cd(t_cmd *cmd)
{
	if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		if (access(getenv("OLDPWD"), R_OK | X_OK) == 0)
			invert_oldpwd(cmd);
		else
		{
			ft_putstr_fd("error, oldpwd can't be established\n", 2);
			return (126);
		}
	}
	else if (access(cmd->args[1], R_OK | X_OK) != 0
		|| chdir(cmd->args[1]) == -1)
	{
		ft_fprintf(2, "minishell: cd: %s: No such file or directory\n",
			cmd->args[1]);
		return (1);
	}
	else
		change_oldpwd(cmd);
	return (0);
}
