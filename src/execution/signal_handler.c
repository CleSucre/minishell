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
			int signal_num = WTERMSIG(status);
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
		int signal_num = WTERMSIG(status);
		if (signal_num == SIGINT)
			return (130);
		else if (signal_num == SIGQUIT)
			return (131);
	}
	return (0);
}
