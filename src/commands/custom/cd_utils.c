/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 05:57:17 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/22 21:22:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_oldpwd(t_minishell *minishell, t_cmd *cmd)
{
	char	buffer[BUFFER_SIZE];

	if (find_table_args(cmd->env, "OLDPWD") != -1)
		return (0);
	add_cmd_env(minishell, "OLDPWD", getcwd(buffer, BUFFER_SIZE));
	cmd->env = minishell->env;
	return (1);
}

static void	print_error(const char *name, struct stat statbuf)
{
	if (stat(name, &statbuf) == 0)
	{
		if ((statbuf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)) == 0)
			ft_fprintf(STDERR_FILENO,
				"minishell: cd: %s: Access denied\n", name);
		else
			ft_fprintf(STDERR_FILENO,
				"minishell: cd: %s: Permission denied\n", name);
	}
}

int	ft_check_access(const char *name)
{
	struct stat	statbuf;

	if (access(name, F_OK) != 0)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: cd: %s: No such file or directory\n", name);
		return (1);
	}
	else if (stat(name, &statbuf) != 0 || !S_ISDIR(statbuf.st_mode))
	{
		ft_fprintf(STDERR_FILENO, "minishell: cd: %s: Not a directory\n",
			name);
		return (1);
	}
	else if (access(name, X_OK) != 0)
		print_error(name, statbuf);
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
