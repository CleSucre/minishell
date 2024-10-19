/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/20 02:26:14 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Count the number of flags in the command
 *
 * @param char **args Arguments of the command
 * @return int Number of flags
 */
static int	count_flags(char **args)
{
	int	i;
	int	j;
	int	count;

	i = 1;
	count = 0;
	while (args[i])
	{
		if (args[i][0] == '-')
		{
			j = 1;
			while (args[i][j] == 'n')
				j++;
			if (args[i][j] == '\0')
				count++;
			else
				break ;
		}
		else
			break ;
		i++;
	}
	return (count);
}

/**
 * @brief Print the arguments of the command
 *
 * @param t_cmd *cmd Command structure
 * @return int Exit code
 */
int	command_echo(t_cmd *cmd)
{
	int		i;
	int		n_flag;

	n_flag = 0;
	i = count_flags(cmd->args);
	if (i)
		n_flag = 1;
	i++;
	while (i < cmd->argc)
	{
		terminal_print(cmd->args[i], 0, cmd->output_fd);
		if (i + 1 < cmd->argc)
			ft_putchar_fd(' ', cmd->output_fd);
		i++;
	}
	terminal_print("", !n_flag, cmd->output_fd);
	return (0);
}
