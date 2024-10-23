/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/17 11:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handle the waiting for child processes
 *
 * @return int Exit status of the last child process (0 if no child process)
 */
int	wait_for_processes(void)
{
	int	status;
	int	res;
	int	signal_num;

	res = 0;
	status = 0;
	while ((wait(&status)) > 0)
	{
		if (WIFEXITED(status))
		{
			res = WEXITSTATUS(status);
			if (res == 255)
				res = 127;
		}
		else if (WIFSIGNALED(status))
		{
			signal_num = WTERMSIG(status);
			if (signal_num == SIGINT)
				return (130);
			else if (signal_num == SIGQUIT)
				return (131);
		}
	}
	return (res);
}

/**
 * @brief Wait for a specific pid
 *
 * @param int pid
 * @return int Exit status of the specific child process
 */
int	wait_for_pid(int pid)
{
	int	status;
	int	res;
	int	signal_num;

	res = 0;
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		res = WEXITSTATUS(status);
		if (res == 255)
			res = 127;
		return (res);
	}
	else if (WIFSIGNALED(status))
	{
		signal_num = WTERMSIG(status);
		if (signal_num == SIGINT)
			return (130);
		else if (signal_num == SIGQUIT)
			return (131);
	}
	return (0);
}
