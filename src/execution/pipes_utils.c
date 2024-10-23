/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 23:51:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/05 23:51:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Close unnecessary file descriptors after command execution
 *
 * @param int in_out[2]
 * @param int *fd
 */
void	close_fds(int in_out[2], int *fd)
{
	if (in_out[0] != STDIN_FILENO)
		close(in_out[0]);
	if (in_out[1] != STDOUT_FILENO)
		close(in_out[1]);
	if (fd[1] != -1)
		close(fd[1]);
	in_out[0] = fd[0];
}

/**
 * @brief Close all file descriptors in the array
 *
 * @param int *to_close
 */
void	close_all_fds(int *to_close)
{
	int	i;

	if (!to_close)
		return ;
	i = 0;
	while (to_close[i] != -1)
	{
		close(to_close[i]);
		to_close[i] = -1;
		i++;
	}
	to_close = NULL;
}

/**
 * @brief Add a file descriptor to the array of file descriptors to close
 *
 * @param int *to_close
 * @param int fd
 */
void	add_fd_to_close(int *to_close, int fd)
{
	int	i;

	i = 0;
	while (to_close[i] != -1)
		i++;
	to_close[i] = fd;
	to_close[i + 1] = -1;
}

/**
 * @brief Setup pipes according to the command
 * 			if its a redirection or a simple pipe for the next cmd
 *
 * @param int *pipes pipe[0] = read, pipe[1] = write
 * @param int *in_out in_out[0] = read, in_out[1] = write, in_out[2] = to_close
 * @param int is_last 1 if its the last command, 0 otherwise
 * @return int 0 on success, -1 on failure
 */
int	setup_pipes(int *pipes, int *in_out, int is_last)
{
	if (!is_last)
	{
		if (pipe(pipes) == -1)
		{
			ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
			return (-1);
		}
		in_out[1] = pipes[1];
		in_out[2] = pipes[0];
	}
	else
	{
		in_out[1] = STDOUT_FILENO;
		in_out[2] = -1;
	}
	return (0);
}

/**
 * @brief Alloc file descriptor to close array
 *
 * @return int*
 */
int	*init_to_close(void)
{
	int	i;
	int	*to_close;

	to_close = malloc(sizeof(int) * MAX_HEREDOC);
	if (!to_close)
	{
		perror("Error: malloc failed");
		return (NULL);
	}
	i = 0;
	while (i < MAX_HEREDOC)
	{
		to_close[i] = -1;
		i++;
	}
	return (to_close);
}
