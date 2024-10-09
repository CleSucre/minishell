/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:34:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/05 19:34:00 by julthoma         ###   ########.fr       */
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
 * @brief Setup pipes according to the command
 * 			if its a redirection or a simple pipe for the next cmd
 *
 * @param int *pipes pipe[0] = read, pipe[1] = write
 * @param int *in_out in_out[0] = read, in_out[1] = write, in_out[2] = to_close
 * @param int is_last 1 if its the last command, 0 otherwise
 * @return int 0 on success, -1 on failure
 */
int setup_pipes(int *pipes, int *in_out, int is_last)
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
	} else {
		in_out[1] = STDOUT_FILENO;
		in_out[2] = -1;
	}
	return (0);
}

/**
 * @brief Copy the contents of a file descriptor to another
 *
 * //TODO: move the function to libft
 *
 * @param int fd_from file descriptor to read from
 * @param int fd_to file descriptor to write to
 * @return ssize_t number of bytes written on success, -1 on failure
 */
ssize_t copy_fd_contents(int fd_from, int fd_to)
{
	char	buf[BUFFER_SIZE];
	ssize_t	bytes_read;
	ssize_t	bytes_written;

	bytes_read = read(fd_from, buf, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		bytes_written = write(fd_to, buf, bytes_read);
		if (bytes_written == -1)
			return (-1);
		bytes_read = read(fd_from, buf, BUFFER_SIZE);
	}
	return (bytes_written);
}

/**
 * @brief Execute the ast respecting pipe operator logic
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, 1 on exit request
 */
int execute_pipe(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out)
{
	int res;

	res = execute_ast(minishell, ast->left, pipes, in_out);
	if (res == 1)
		return (res); //TODO: check if this is the correct behavior, should it return 0 or res?
	res = execute_ast(minishell, ast->right, pipes, in_out);
	if (res == 1)
		return (res); //TODO: check if this is the correct behavior, should it return 0 or res?
	return (0);
}

/**
 * @brief Redirect the of a file to the input of the command
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, -1 on failure
 */
int	execute_redirect_input(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out)
{
	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no file specified\n", STDERR_FILENO);
		return (1);
	}
	//TODO: check if the file exists and is readable
	in_out[0] = open(ast->right->value[0], O_RDONLY);
	if (in_out[0] == -1)
	{
		ft_putstr_fd("Error: open failed\n", STDERR_FILENO);
		return (1);
	}
	//TODO: FIX THIS
	execute_ast(minishell, ast->left, pipes, in_out);
	close(in_out[0]);
	return (0);
}

/**
 * @brief Execute the ast respecting heredoc logic
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, -1 on failure
 */
int	execute_heredoc(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out)
{
	(void)minishell;
	(void)pipes;
	(void)in_out;
	ft_fprintf(STDERR_FILENO, "heredoc: %s\n", ast->right->value[0]);
	return (0);
}

/**
 * @brief Redirect the output of the command to a file
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @param int append
 * @return int 0 on success, -1 on failure
 */
int	execute_redirect_output(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out, int append)
{
	int	file_fd;

	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no file specified\n", STDERR_FILENO);
		return (1);
	}
	//TODO: check if the file exists, if not create it
	if (access(ast->right->value[0], W_OK) != 0)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: Permission denied\n", ast->right->value[0]);
		return (1); //TODO: dont stop the all execution
	}
	execute_ast(minishell, ast->left, pipes, in_out);
	if (append)
		file_fd = open(ast->right->value[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		file_fd = open(ast->right->value[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
	{
		ft_putstr_fd("Error: open failed\n", STDERR_FILENO);
		return (1); //TODO: dont stop the all execution
	}
	copy_fd_contents(in_out[0], file_fd);
	close(file_fd);
	close_fds(in_out, pipes);
	wait_for_processes();
	return (0); //TODO: check if this is the correct behavior
}
