/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 07:50:12 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handle the signal
 *
 * @param int sig
 * @eturn void
 */
void	handle_signal(int sig)
{
	if (sig == SIGUSR1)
		ft_putstr_fd("Received SIGUSR1 signal\n", STDOUT_FILENO);
}

/**
 * @brief Ugly custom sleep function.
 * 		We are not allowed to use sleep functions...
 *
 * @param int n
 */
void	custom_sleep(int n)
{
	int	i;

	i = 0;
	while (i < n * 1000000)
		i++;
}

/**
 * @brief Get the exit status of the command
 *
 * @param pid_t pid
 * @param int *status
 * @return
 */
static int	waitpid_with_timeout(pid_t pid)
{
	int	ret;
	int status;

	while (1)
	{
		ft_fprintf(STDERR_FILENO, "Sleeping...\n");
		custom_sleep(100);
		ft_fprintf(STDERR_FILENO, "Waking up...\n");
		ret = waitpid(pid, &status, WNOHANG);
		if (ret == -1)
		{
			perror("waitpid");
			return (-1);
		}
		else if (ret > 0)
			return (ret);
		return (ETIMEDOUT);
	}
}

/**
 * @brief Execute the command given in input
 *
 * @param t_cmd *cmd
 * @return int 0 on success, 1 on failure
 */
static int execute_path(t_cmd *cmd)
{
	pid_t pid;
	struct sigaction sa;
	int pipefd[2];
	char buf;

	sa.sa_handler = handle_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("sigaction");
		return 1;
	}
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return 1;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		cmd->exit_status = 1;
		return 0;
	}
	else if (pid == 0)
	{
		close(pipefd[1]);
		if (sigaction(SIGUSR1, &sa, NULL) == -1)
		{
			perror("sigaction");
			exit(EXIT_FAILURE);
		}
		if (read(pipefd[0], &buf, 1) != 1)
		{
			perror("read");
			close(pipefd[0]);
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		dup2(cmd->input, STDIN_FILENO);
		dup2(cmd->output, STDOUT_FILENO);
		if (cmd->input != STDIN_FILENO)
			close(cmd->input);
		if (cmd->output != STDOUT_FILENO)
			close(cmd->output);
		if (cmd->fd_to_close != -1)
			close(cmd->fd_to_close);
		cmd->pid = getpid();
		fprintf(stderr, "pid: %d\n", cmd->pid);
		if (execve(cmd->cmd_exec, cmd->argv, cmd->env) == -1)
		{
			fprintf(stderr, "minishell: command not found: %s\n", cmd->cmd_name);
			cmd->exit_status = 127;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		close(pipefd[0]);
		// Notify the child process by writing a byte to the pipe
		if (write(pipefd[1], "x", 1) != 1)
		{
			perror("write");
			close(pipefd[1]);
			return 1;
		}
		close(pipefd[1]);
		if (kill(pid, SIGUSR1) == -1)
		{
			perror("kill");
			return 1;
		}

		while (waitpid_with_timeout(pid) == ETIMEDOUT)
		{
			read(STDIN_FILENO, &buf, 1);
			if (buf == 4)
			{
				printf("Ctrl+D pressed\n");
				//send signal to child
				kill(pid, SIGKILL);
				if (WIFEXITED(cmd->exit_status))
					cmd->exit_status = WEXITSTATUS(cmd->exit_status);
				break ;
			}
			else
				write(STDOUT_FILENO, &buf, 1);
		}
	}

	return 0;
}

static int	execute_cmd(t_minishell *minishell, t_cmd *cmd)
{
	int	custom_cmd_res;

	if (!cmd)
		return (0);
	custom_cmd_res = execute_custom_command(minishell, cmd);
	if (custom_cmd_res)
		return (custom_cmd_res);
	execute_path(cmd);
	return (1);
}

/**
 * @brief Execute the ast
 *
 * @param t_minishell *minishell
 * @param t_ast *ast
 * @param int *pipe_fd
 * @param int fd_to_close
 * @return int 1 on success, 0 on failure
 */
static int	execute_cmds(t_minishell *minishell, t_ast *ast, int *pipe_fd)
{
	t_cmd			*cmd;
	unsigned int	i;
	int				fd[2];
	unsigned int	len;

	if (!ast)
		return (0);
	len = ast_count_type(ast, FULL_COMMAND);
	i = 0;
	while (ast)
	{
		if (ast->type == FULL_COMMAND)
		{
			if (i < len - 1)
			{
				pipe(fd);
				pipe_fd[1] = fd[1];
				cmd = load_command(minishell, ast->children, pipe_fd, fd[0]);
				if (cmd)
				{
					execute_cmd(minishell, cmd);
					free_cmd(cmd);
					close(fd[1]);
				}
				if (pipe_fd[0] != STDIN_FILENO)
					close(pipe_fd[0]);
				pipe_fd[0] = fd[0];
			}
			else
			{
				pipe_fd[1] = STDOUT_FILENO;
				if (len > 1)
					cmd = load_command(minishell, ast->children, pipe_fd,
							fd[1]);
				else
					cmd = load_command(minishell, ast->children, pipe_fd, -1);
				if (cmd)
				{
					execute_cmd(minishell, cmd);
					free_cmd(cmd);
				}
				if (len > 1)
				{
					close(fd[0]);
					close(fd[1]);
				}
				if (pipe_fd[0] != STDIN_FILENO)
					close(pipe_fd[0]);
				if (pipe_fd[1] != STDOUT_FILENO)
					close(pipe_fd[1]);
			}
			i++;
		}
		ast = ast->next;
	}
	return (0);
}

/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return int 1 on success, 0 on failure
 */
int	execute(t_minishell *minishell, char *input)
{
	t_ast	*ast;
	int		res;
	int		pipe_fd[2];

	if (ft_strlen(input) == 0)
		return (0);
	input = ft_strtrim(input, WHITESPACES);
	if (!input)
		return (0);
	debug_execution(input);
	if (ft_isprint(*input))
		history_add(minishell, input, 1);
	ast = parse_input(minishell, input);
	if (!ast)
	{
		free(input);
		return (0);
	}
	pipe_fd[0] = STDIN_FILENO;
	pipe_fd[1] = STDOUT_FILENO;
	res = execute_cmds(minishell, ast, pipe_fd);
	free_ast(ast);
	free(input);
	return (res);
}
