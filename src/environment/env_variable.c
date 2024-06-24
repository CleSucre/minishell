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
 * @brief Locate the executable path for a command within the system's PATH.
 *
 * @param char *cmd The command to locate.
 * @param char **env Environment variables, including PATH.
 * @return char* Full path to the executable, or NULL if not found.
 */
char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*path;
	char	**paths;
	char	*tmp;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	paths = ft_split(get_var_value_const(env, (char *)"PATH"), ":");
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, X_OK) == 0)
		{
			ft_freetab(paths);
			return (path);
		}
		free(path);
	}
	ft_freetab(paths);
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
 * @brief Get a variable value length without any allocation
 *
 * @param char **env environment variables
 * @param char *var variable to get the length
 * @return int length of the variable
 */
int	get_var_len(char **env, char *var)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (ft_strlen(env[i] + len + 1));
		i++;
	}
	return (len);
}

static char	*ft_strjoin_char(char *s1, char c)
{
	char	*res;
	int		i;

	if (!s1)
	{
		res = ft_calloc(2, sizeof(char));
		res[0] = c;
		return (res);
	}
	res = ft_calloc(ft_strlen(s1) + 2, sizeof(char));
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	res[i] = c;
	free(s1);
	return (res);
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
	int		i;
	int		j;
	char	*res;
	char	*tmp;

	res = NULL;
	i = 0;
	while (str[i])
	{
		j = 0;
		if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
		{
			while (ft_isalnum(str[i + j + 1]))
				j++;
			tmp = ft_substr(str, i + 1, j);
			res = ft_strjoin_free(res, get_var_value(env, tmp));
			free(tmp);
			i += j + 1;
		}
		else
		{
			res = ft_charjoin(res, str[i]);
			i++;
		}
	}
	return (res);
}

/**
 * @brief Replace the variables in a string by their values
 * 			using the env variables loaded in t_minishell struct
 *
 * 	TODO: give t_minishell and just code it :)
 *
 * @param char *str
 * @return t_ast *
 */
char	*extract_variables(t_minishell *minishell, char *str)
{
	char	*trimmed;
	char	*text;

	trimmed = ft_strtrim(str, "\"");
	if (!trimmed)
		return (NULL);
	text = replace_variables(minishell->env, trimmed);
	free(trimmed);
	return (text);
}