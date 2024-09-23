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
 * @param minishell
 * @param search
 */
static void	invert_oldpwd(t_minishell *minishell)
{
	int		pwd;
	char	*name;
	char	*buff_name;
	int		oldpwd;
	char	buffer[BUFFER_SIZE];

	oldpwd = find_table_args(minishell->env, "OLDPWD");
	pwd = find_table_args(minishell->env, "PWD");
//	ft_fprintf(2, "oldpwd [%s], pwd [%s]\n\n\n\n", minishell->env[oldpwd], minishell->env[pwd]);
	if (oldpwd == -1 || pwd == -1)
		return ;
	name = strcpy_passing_char(minishell->env[oldpwd], 7);
	if (chdir(name) == -1)
		ft_putstr_fd("Error accessing oldpwd\n", 2);
	free(name);
	clear_string(minishell->env[oldpwd]);
	buff_name = ft_strjoin("OLD", minishell->env[pwd]);
	ft_strlcpy(minishell->env[oldpwd], buff_name,
		(int)ft_strlen(minishell->env[pwd]) + 4);
	free(buff_name);
	clear_string(minishell->env[pwd]);
	name = getcwd(buffer, BUFFER_SIZE);
	buff_name = ft_strjoin("PWD=", name);
	ft_strlcpy(minishell->env[pwd], buff_name, (int)ft_strlen(name) + 5);
	free(buff_name);
//	ft_fprintf(2, "AFTER oldpwd [%s], pwd [%s]\n", minishell->env[oldpwd], minishell->env[pwd]);
	return ;
}

static void	change_oldpwd(t_minishell *minishell)
{
	int		pwd;
	char	*name;
	char	*buff_name;
	int		oldpwd;
	char	buffer[BUFFER_SIZE];

	oldpwd = find_table_args(minishell->env, "OLDPWD");
	pwd = find_table_args(minishell->env, "PWD");
	if (oldpwd == -1 || pwd == -1)
		return ;
//	ft_fprintf(2, "BEFORE CHANGE\noldpwd [%s], pwd [%s]\n\n\n\n", minishell->env[oldpwd], minishell->env[pwd]);
	clear_string(minishell->env[oldpwd]);
	buff_name = ft_strjoin("OLD", minishell->env[pwd]);
	ft_strlcpy(minishell->env[oldpwd], buff_name,
		(int)ft_strlen(minishell->env[pwd]) + 4);
	free(buff_name);
	clear_string(minishell->env[pwd]);
	name = getcwd(buffer, BUFFER_SIZE);
	buff_name = ft_strjoin("PWD=", name);
	ft_strlcpy(minishell->env[pwd], buff_name, (int)ft_strlen(name) + 5);
	free(buff_name);
//	ft_fprintf(2, "AFTER CHANGE\noldpwd [%s], pwd [%s]\n", minishell->env[oldpwd], minishell->env[pwd]);
	return ;
}

/**
 * @brief Change the current working directory
 * @param cmd The command to execute.
 * By default, as if -L is used
 * @return int 0 if the command was executed successfully, 1 otherwise.
 */
int	command_cd(t_minishell *minishell, t_cmd *cmd)
{
	if (ft_strcmp(cmd->argv[1], "-") == 0)
	{
		if (access(getenv("OLDPWD"), R_OK | X_OK) == 0)
			invert_oldpwd(minishell);
		else
		{
			ft_putstr_fd("error, oldpwd can't be established\n", 2);
			minishell->exit_code = 126;
			return (1);
		}
	}
	else if (access(cmd->argv[1], R_OK | X_OK) != 0
		|| chdir(cmd->argv[1]) == -1)
	{
		ft_fprintf(2, "minishell: cd: %s: No such file or directory\n",
			cmd->argv[1]);
		minishell->exit_code = 1;
		return (1);
	}
	else
		change_oldpwd(minishell);
	minishell->exit_code = 0;
	return (0);
}
