/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 09:38:12 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 09:41:54 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define TIMEOUT 1 // Timeout in seconds

// Signal handler for SIGALRM
void handle_alarm(int sig)
{
    if (sig == SIGALRM)
    {
        write(STDOUT_FILENO, "\nRead operation timed out\n", 25);
    }
}

// Function to read with a timeout
ssize_t read_with_timeout(int fd, void *buf, size_t count, unsigned int timeout)
{
    struct sigaction sa;
    ssize_t bytes_read;

    // Set up the signal handler for SIGALRM
    sa.sa_handler = handle_alarm;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
        perror("sigaction");
        return -1;
    }

    // Set the alarm
    alarm(timeout);

    // Perform the read operation
    bytes_read = read(fd, buf, count);

    // Cancel the alarm
    alarm(0);

    if (bytes_read == -1 && errno == EINTR)
    {
        // Read was interrupted by SIGALRM
        errno = ETIMEDOUT;
    }

    return bytes_read;
}

int main()
{
    char buffer[100];
    ssize_t bytes_read;

    printf("You have %d seconds to type something: ", TIMEOUT);
    fflush(stdout);

	while (1)
	{
		// Read from stdin with a timeout
		bytes_read = read_with_timeout(STDIN_FILENO, buffer, sizeof(buffer) - 1, TIMEOUT);

		if (bytes_read == -1)
		{
			if (errno == ETIMEDOUT)
			{
				printf("\nTimeout occurred. No input received.\n");
			}
			else
			{
				perror("read");
			}
		}
		else
		{
			buffer[bytes_read] = '\0';
			printf("\nYou typed: %s\n", buffer);
		}
	}

    return 0;
}

