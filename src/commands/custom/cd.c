/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/10/22 21:22:05 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/22 21:22:05 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

/**
 * @brief function to invert olwdpwd and pwd when cd - is used
 * @param t_cmd *cmd
 * @param search
 */
void	invert_oldpwd(t_cmd *cmd)
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

/**
 * @brief Change the current working directory
 *
 * @param t_cmd *cmd
 */
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

static int	update_env_var(char **env, const char *var_name,
							const char *new_value)
{
	int		position;
	char	*buf_name;
	char	*full_value;

	position = find_table_args(env, (char *)var_name);
	if (position == -1)
		return (-1);
	free(env[position]);
	buf_name = ft_strjoin(var_name, "=");
	if (!buf_name)
		return (-1);
	full_value = ft_strjoin(buf_name, new_value);
	free(buf_name);
	if (!full_value)
		return (-1);
	env[position] = ft_strdup(full_value);
	free(full_value);
	if (!env[position])
		return (-1);
	return (0);
}

static int	go_home(t_cmd *cmd)
{
	char	*home_value;
	char	*pwd_value;

	home_value = get_env_value(cmd, "HOME", "minishell: cd: HOME not set\n");
	if (!home_value)
		return (1);
	if (ft_check_access(home_value) != 0)
		return (1);
	pwd_value = ft_strdup(get_env_value(cmd, "PWD",
				"minishell: cd: PWD not set\n"));
	if (!pwd_value)
		return (1);
	if (update_env_var(cmd->env, "OLDPWD", pwd_value) != 0)
	{
		free(pwd_value);
		return (1);
	}
	if (update_env_var(cmd->env, "PWD", home_value) != 0)
	{
		free(pwd_value);
		return (1);
	}
	free(pwd_value);
	chdir(home_value);
	return (0);
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
		return (go_home(cmd));
	minus = cd_minus(cmd);
	if (minus != 1)
		return (minus);
	else if (ft_check_access(cmd->args[1]) != 0 || chdir(cmd->args[1]) == -1)
		return (1);
	else
		move_dir(cmd);
	return (0);
}
