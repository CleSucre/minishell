/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 06:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	str_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_llong(char *str, long long *res)
{
	char	*endptr;

	(void)res;
	str = noptozero(str);
	if (ft_strlen(str) > 20)
		return (1);
	*res = ft_strtoll(str, &endptr, 10);
	if (*endptr != '\0')
		return (1);
	return (0);
}

/**
 * @brief Exit the shell with a status code (default 0)
 *
 * @param t_cmd *cmd Command structure
 * @return int Exit code
 */
int	command_exit(t_cmd *cmd)
{
	long long	status;

	status = 0;
	if (!cmd->args[1])
	{
		cmd->exit_signal = 1;
        return (0);
	}
	if (check_llong(cmd->args[1], &status) || str_is_digit(cmd->args[1]) == 0)
	{
		ft_fprintf(2, "minishell: exit: %s: numeric argument required\n",
			cmd->args[1]);
		cmd->exit_signal = 1;
		return (2);
	}
	if (cmd->argc > 2)
	{
		ft_fprintf(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	cmd->exit_signal = 1;
	return (status % 256);
}
