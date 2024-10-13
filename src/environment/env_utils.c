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
 * @brief Reload the environment variables.
 * 			Used to update the PWD variable with chdir.
 *
 * @param char **env Environment variables to reload
 * @return int 0 if success, 1 otherwise
 */
int	reload_env(char **env)
{
	const char	*pwd;

	pwd = get_var_value_const(env, "PWD");
	if (pwd == NULL)
	{
		ft_putstr_fd("Error accessing PWD\n", STDERR_FILENO);
		return (1);
	}
	if (chdir(pwd) == -1)
	{
		ft_putstr_fd("Error accessing PWD\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}
