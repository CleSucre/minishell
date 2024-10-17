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
 * @brief Print the last argument with a special format if -n flag is present
 *
 * @param int output Output file descriptor
 */
static void	print_n_flag(int output)
{
	ft_putstr_fd(BACKGROUND_WHITE, output);
	ft_putstr_fd(TEXT_BLACK, output);
	terminal_print("%", 0, output);
	ft_putstr_fd(RESET, output);
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

	i = 1;
	n_flag = 0;
	if (cmd->argc > 1 && ft_strncmp(cmd->args[1], "-n", 2) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (i < cmd->argc)
	{
		terminal_print(cmd->args[i], 0, cmd->output_fd);
		if (i + 1 < cmd->argc)
			ft_putchar_fd(' ', cmd->output_fd);
		i++;
	}
	if (n_flag)
		print_n_flag(cmd->output_fd);
	terminal_print("", 1, cmd->output_fd);
	return (0);
}
