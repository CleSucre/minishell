/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 06:32:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/14 06:32:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the path and free the paths if correct
 *
 * @param char *path
 * @param char **paths
 * @return int 1 if the path is correct, 0 otherwise
 */
static int	check_path(char *path)
{
	if (access(path, X_OK) == 0)
		return (1);
	free(path);
	return (0);
}

/**
 * @brief Get the path of the command to execute
 *
 * @param char *cmd
 * @param char **envp
 * @return char* the path of the command
 */
char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	**paths; // TODO: Change le nom d une des deux variable je t en pris mdrrr path et paths quoi xD
	char	*tmp;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	paths = ft_split(envp[i] + 5, ":");
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (check_path(path))
		{
			ft_tabfree(paths);
			return (path);
		}
	}
	ft_tabfree(paths);
	return (NULL);
}

/**
 * @brief Get a duplicate of the value of a variable
 *
 * @param char **env environment variables
 * @param char *var variable to get the value
 * @return char * value of the variable
 */
char	*get_var_value(char **env, char *var)
{
	int		i;
	int		len;
	char	*value;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
		{
			value = ft_strdup(env[i] + len + 1);
			return (value);
		}
		i++;
	}
	return (ft_strdup(""));
}

/**
 * @brief Get a variable value without any allocation
 *
 * @param char **env environment variables
 * @param char *var variable to get the value
 * @return const char * value of the variable
 */
const char	*get_var_value_const(char **env, char *var)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return ("");
}

/**
 * @brief Replace the variables in a string by their values
 * 			using the env variables (only for the $VARNAME format)
 *
 * @param t_minishell *minishell
 * @param char *str
 * @return char* string with replaced variables
 */
char	*replace_variables(char **env, char *str)
{
	size_t	i;
	int		j;
	char	*res;
	char	*tmp;

	res = NULL;
	i = 0;
	while (ft_strlen(str) > i)
	{
		if (str[i] == '$')
		{
			j = i + 1;
			while (ft_isalnum(str[j]))
				j++;
			tmp = ft_substr(str, i + 1, j - i - 1);
			res = ft_strjoin_free(res, get_var_value(env, tmp));
			free(tmp);
			i = j;
		}
		else
			res = ft_strjoin_char(res, str[i++]);
	}
	if (!res)
		res = ft_strdup("");
	return (res);
}
