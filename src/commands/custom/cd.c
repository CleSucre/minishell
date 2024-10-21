/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
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

static void	move_dir(t_cmd *cmd)
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

static int	go_home(t_cmd *cmd)
{
	char	*oldpwd;
	char	*pwd;
	char	*buf_name;
	int		position;

	oldpwd = (char *)get_var_value_const(cmd->env, "PWD");
	position = find_table_args(cmd->env, "OLDPWD");
	if (position == -1)
		return (1);
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
	return (0);
}

int ft_check_access(const char *name)
{
	struct stat statbuf;


	if (access(name, F_OK) != 0)
	{
		ft_fprintf(STDERR_FILENO, "minishell: cd: %s: No such file or directory\n", name);
		return (1);
	}
	else if (stat(name, &statbuf) != 0 || !S_ISDIR(statbuf.st_mode)) {
		ft_fprintf(STDERR_FILENO, "minishell: cd: %s: Not a directory\n", name);
		return (1);
	}
	else if (access(name, X_OK) != 0) {
		if (stat(name, &statbuf) == 0) {
			if ((statbuf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)) == 0)
				ft_fprintf(STDERR_FILENO, "minishell: cd: %s: Access denied\n", name);
			else
				ft_fprintf(STDERR_FILENO, "minishell: cd: %s: Permission denied\n", name);
		}
	}
	return (0);
}

int	cd_minus(t_cmd *cmd)
{
	char	**oldpwd;
	int		position;

	position = find_table_args(cmd->env, "OLDPWD");
	if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		oldpwd = ft_split_quote(cmd->env[position], "=", "\"\'");
		if (ft_check_access(oldpwd[1]) == 0)
		{
			invert_oldpwd(cmd);
			ft_printf("%s\n", oldpwd[1]);
			ft_tabfree(oldpwd);
			return (0);
		}
		else
		{
			ft_tabfree(oldpwd);
			return (126);
		}
	}
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
	int	minus;

	check_oldpwd(minishell, cmd);
	if (cmd->argc > 2)
	{
		ft_fprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}
	else if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
	{
		return (go_home(cmd));
	}
	minus = cd_minus(cmd);
	if (minus != 1)
		return (minus);
	else if (ft_check_access(cmd->args[1]) != 0 || chdir(cmd->args[1])
		== -1)
		return (1);
	else
		move_dir(cmd);
	return (0);
}
