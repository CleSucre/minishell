//
// Created by clesucre on 7/7/24.
//

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
