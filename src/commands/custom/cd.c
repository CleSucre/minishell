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
	free(cmd->env[oldpwd]);
	buff_name = ft_strjoin("OLD", cmd->env[pwd]);
	if (!buff_name)
		return ;
	cmd->env[oldpwd] = ft_strdup(buff_name);
	free(buff_name);
	free(cmd->env[pwd]);
	name = getcwd(buffer, BUFFER_SIZE);
	buff_name = ft_strjoin("PWD=", name);
	cmd->env[pwd] = ft_strdup(buff_name);
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
	free(cmd->env[oldpwd]);
	buff_name = ft_strjoin("OLD", cmd->env[pwd]);
	if (!buff_name)
		return ;
	cmd->env[oldpwd] = ft_strdup(buff_name);
	free(buff_name);
	free(cmd->env[pwd]);
	name = getcwd(buffer, BUFFER_SIZE);
	buff_name = ft_strjoin("PWD=", name);
	cmd->env[pwd] = ft_strdup(buff_name);
	free(buff_name);
}





static void go_home(t_cmd *cmd)
{
	char *oldpwd;
	char *pwd;
	char *buf_name;
	int position;

	oldpwd = (char *)get_var_value_const(cmd->env, "PWD");
	position = find_table_args(cmd->env, "OLDPWD");
	if (position == -1)
		return ;
	free(cmd->env[position]);
	buf_name = ft_strjoin("OLDPWD=", oldpwd);
	cmd->env[position] = ft_strdup(buf_name);
	free(buf_name);
	position = find_table_args(cmd->env, "PWD");
	free(cmd->env[position]);
	pwd = (char *)get_var_value_const(cmd->env, "HOME");
	buf_name = ft_strjoin("PWD=", pwd);
	cmd->env[position] = ft_strdup(buf_name);
	free(buf_name);
	chdir(pwd);
}


static int check_oldpwd(t_minishell *minishell, t_cmd *cmd)
{
	char **tmp;

	if (find_table_args(minishell->env, "OLDPWD") != -1)
		return (0);
	tmp = ft_tabdup((const char **)cmd->env);
	add_cmd_env(minishell, "OLDPWD", getcwd(NULL, 0));
	cmd->env = ft_tabinsert(tmp, ft_strjoin("OLDPWD=", getcwd(NULL, 0)),
		ft_tablen((const char **)cmd->env) - 1);
	return (1);
}

/**
 * @brief Change the current working directory
 * @param cmd The command to execute.
 * By default, as if -L is used
 * @return int 0 if the command was executed successfully, 1 otherwise.
 */
int	command_cd(t_minishell *minishell, t_cmd *cmd)
{
	char	**oldpwd;
	int		position;

	check_oldpwd(minishell, cmd);
	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
	{
		go_home(cmd);
		return (0);
	}
	position = find_table_args(cmd->env, "OLDPWD");
	oldpwd = ft_split_quote(cmd->env[position], "=", "\"\'");
	if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		if (access(oldpwd[1], R_OK | X_OK) == 0)
			invert_oldpwd(cmd);
		else
		{
			ft_tabfree(oldpwd);
			ft_putstr_fd("error, oldpwd can't be established\n", 2);
			return (126);
		}
	}
	else if	(access(cmd->args[1], R_OK | X_OK) != 0
		|| chdir(cmd->args[1]) == -1)
	{
		ft_fprintf(2, "minishell: cd: %s: No such file or directory\n",
			cmd->args[1]);
		ft_tabfree(oldpwd);
		return (1);
	}
	else
		change_oldpwd(cmd);
	ft_tabfree(oldpwd);
	return (0);
}
