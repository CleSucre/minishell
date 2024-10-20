/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:13:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/09 18:13:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handle the signal
 *
 * @param int sig
 * @return void
 */
void	handle_signal(int sig)
{
	if (sig == SIGUSR1)
		ft_putstr_fd("Received SIGUSR1 signal\n", STDOUT_FILENO);
}

/**
 * @brief Handle the waiting for child processes
 *
 * @param int *status
 * @return int WEXITSTATUS of the child process (0 if no child process)
 */
int	wait_for_processes(void)
{
	int	status;
	int	res;

	status = 0;
	while ((wait(&status)) > 0)
		;
	if (WIFEXITED(status))
	{
		res = WEXITSTATUS(status);
		if (res == 255)
			res = 127;
		return (res);
	}
	else
		return (0);
}

/**
 * @brief Wait for a specific pid
 *
 * @param int pid
 * @return int WEXITSTATUS of the child process
 */
int	wait_for_pid(int pid)
{
	int	status;
	int	res;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		res = WEXITSTATUS(status);
		if (res == 255)
			res = 127;
		return (res);
	}
	else
		return (0);
}
