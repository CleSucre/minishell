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
 * @brief Handle the waiting for next processes
 *
 * @param int *status
 * @return int WEXITSTATUS on success
 */
int	wait_for_processes(int *status)
{
	while ((wait(status)) > 0)
		;
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	else
		return (0);
}
