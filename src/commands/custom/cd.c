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
	int		verif_home;

	oldpwd = (char *)get_var_value_const(cmd->env, "PWD");
	position = find_table_args(cmd->env, "OLDPWD");
	verif_home = find_table_args(cmd->env, "HOME");
	if (position == -1 || verif_home == -1 || ft_strlen(cmd->env[verif_home]) == 0)
	{
		printf("verif home = %d\n", verif_home);
		ft_fprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
		return (1);
	}
	ft_fprintf(STDERR_FILENO, "verif home len = %d\n", ft_strlen(cmd->env[verif_home]));
	if (ft_check_access((char *)get_var_value_const(cmd->env, "HOME")) != 0)
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
	else if (ft_check_access(cmd->args[1]) != 0 || chdir(cmd->args[1]) == -1)
		return (1);
	else
		move_dir(cmd);
	return (0);
}
