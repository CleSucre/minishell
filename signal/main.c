/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:20:34 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 07:40:09 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <termios.h>
#include <errno.h>

// Signal handler function
void handle_sigusr1(int sig)
{
	if (sig == SIGUSR1)
	{
		write(STDOUT_FILENO, "Received SIGUSR1 signal\n", 24);
	}
}

void use_termios(char **envp);

void	custom_sleep(int n)
{
	int i;

	i = 0;
	while (i < n * 1000000)
		i++;
}

int waitpid_with_timeout(pid_t pid)
{
	int ret;
	int status;

	while (1)
	{
		custom_sleep(100);
		ret = waitpid(pid, &status, WNOHANG);
		if (ret == -1)
		{
			perror("waitpid");
			return -1;
		}
		else if (ret > 0)
		{
			// Child process exited
			return ret;
		}

		// Child process is still running
		return ETIMEDOUT;
	}
}

int		pipefd[2];
pid_t	pid;

int execute_cmd(char *cmd, char **args, char **envp)
{
	struct sigaction sa;
	char buf;

	// Set up the sigaction structure to handle SIGUSR1
	sa.sa_handler = handle_sigusr1;
	sa.sa_flags = 0; // No special flags
	sigemptyset(&sa.sa_mask); // No additional signals to block

	// Set up the signal handler
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	// Create a pipe
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	// Fork a child process
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		// Child process
		close(pipefd[1]); // Close the write end of the pipe

		printf("Child process (PID: %d) waiting for SIGUSR1...\n", getpid());

		// Set up the signal handler in the child process
		if (sigaction(SIGUSR1, &sa, NULL) == -1)
		{
			perror("sigaction");
			exit(EXIT_FAILURE);
		}

		// Notify the parent that the child is ready
		read(pipefd[0], &buf, 1); // Read a byte from the pipe to synchronize
		close(pipefd[0]); // Close the read end of the pipe

		// Execute the command provided as arguments
		if (execvp(cmd, args) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		// Parent process
		close(pipefd[0]); // Close the read end of the pipe

		// Notify the child process by writing a byte to the pipe
		write(pipefd[1], "x", 1);
		close(pipefd[1]); // Close the write end of the pipe

		// Send SIGUSR1 to the child process
		printf("Parent process (PID: %d) sending SIGUSR1 to child process (PID: %d)\n", getpid(), pid);
		if (kill(pid, SIGUSR1) == -1)
		{
			perror("kill");
			exit(EXIT_FAILURE);
		}

		while (waitpid_with_timeout(pid) == ETIMEDOUT)
		{
			read(STDIN_FILENO, &buf, 1);
			if (buf == 4)
			{
				printf("Ctrl+D pressed\n");
				//send signal to child
				kill(pid, SIGKILL);
				break ;
			}
		}
	}
	return 0;
}

typedef struct s_term
{
	struct termios original_termios;
} t_term;

/**
 * @brief Save the original termios settings
 *
 * @param t_term *term
 * @return void
 */
static void save_termios(t_term *term)
{
	if (tcgetattr(STDIN_FILENO, &term->original_termios) == -1)
		perror("tcgetattr");
}

/**
 * @brief Enable termios to read char, and catch exhaust sequence
 *
 * @param t_term *term
 * @return void
 */
void enable_termios(t_term *term)
{
	struct termios new_termios;

	save_termios(term);
	new_termios = term->original_termios;
	new_termios.c_lflag &= ~(ECHO | ICANON | ISIG);
	new_termios.c_cc[VERASE] = 127;
	new_termios.c_iflag &= ~(ICRNL | INLCR | IGNCR);
	new_termios.c_cc[VMIN] = 1;
	new_termios.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1)
		perror("tcsetattr");
}

/**
 * @brief Restore default settings of termios
 *
 * @param t_term *termios
 * @return void
 */
void disable_termios(t_term *term)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term->original_termios) == -1)
		perror("tcsetattr");
}

void    sigint_muted(int signal)
{
	printf("signal : %d\n", signal);
}

/**
 * @brief Use termios to read char
 *
 * @param char **envp
 * @return void
 */
void use_termios(char **envp)
{
	int i;
	char c;
	char *input;
	char **args;

	input = (char *)malloc(1 * sizeof(char));
	if (input == NULL)
	{
		printf("Error: Unable to allocate memory\n");
		exit(EXIT_FAILURE);
	}
	input[0] = '\0';
	args = (char **)malloc(2 * sizeof(char *));
	if (args == NULL)
	{
		printf("Error: Unable to allocate memory\n");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (1)
	{
		read(STDIN_FILENO, &c, 1);
		if (c == 13)
		{
			if (strlen(input) <= 0)
				continue;
			printf("\nInput: %s\n", input);
			args[0] = input;
			args[1] = NULL;

			execute_cmd(input, args, envp);
			i = 0;
			input[0] = '\0';
			continue;
		}
		else if (c == 3)
		{
			printf("Ctrl+C pressed\n");
			break;
		}
		else if (c == 4)
		{
			printf("Ctrl+D pressed\n");
			//send signal to child
			kill(pid, SIGKILL);
			continue ;
		}
		// Reallocate memory for input
		input = (char *)realloc(input, (i + 1) * sizeof(char));
		if (input == NULL)
		{
			printf("Error: Unable to allocate memory\n");
			exit(EXIT_FAILURE);
		}
		write(STDOUT_FILENO, &c, 1);
		input[i] = c;
		i++;
		input[i] = '\0';
	}
	free(input);
	free(args);
}

int main(int argc, char **argv, char **envp)
{
	t_term term;

	(void)argc;
	(void)argv;

	enable_termios(&term);
	use_termios(envp);
	disable_termios(&term);
	return 0;
}
