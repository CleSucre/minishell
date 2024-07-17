/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 13:11:54 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/10 09:13:14 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Close the pipe properly depending on the number of commands
 *
 * @param int cmds_len
 * @param int fd[2]
 * @param int input
 * @param int output
 */
void	close_pipe(int cmds_len, int fd[2], int input, int output)
{
	if (cmds_len < 3)
	{
		close(fd[0]);
		close(fd[1]);
	}
	close(input);
	close(output);
}
