/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 09:53:22 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Close unnecessary file descriptors after command execution
 *
 * @param int in_out[]
 * @param int fd[]
 */
static void	close_fds(int in_out[2], int *fd)
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
 * @param int state 0 if pipe, 1 if last command, 2 if redirection
 * @return int 0 on success, -1 on failure
 */
static int setup_pipes(int *pipes, int *in_out, int is_last)
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
 * @brief Execute the command given in input
 *
 * //TODO: move the function to the correct file, where? IDK :)
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *in_out
 * @return 0 on success -1 on exit request
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
	return (0);
}

/**
 * @brief Execute the ast, minishell->exit_code will be set to
 * 			the return value of the last command
 * 		Always start by commands on the left, then the right
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @return Last command exit code
 */
static int	execute_ast(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out)
{
	int	res;
	int status;
	int file_fd;

	if (!ast)
		return (0);
	if (ast->type == AST_COMMAND)
	{
		if (setup_pipes(pipes, in_out, ast->is_last) == -1)
		{
			ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
			return (1);
		}
		res = execute_cmd(minishell, ast, in_out);
		close_fds(in_out, pipes);
		if (res == 1) //TODO: check if this is the correct behavior, should it return 0 or res?
			return (res);
	}
	else if (ast->type == AST_PIPE)
	{
		res = execute_ast(minishell, ast->left, pipes, in_out);
		if (res == 1)
			return (res); //TODO:
		res = execute_ast(minishell, ast->right, pipes, in_out);
		if (res == 1)
			return (res);
	}
	else if (ast->type == AST_AND)
	{
		res = execute_ast(minishell, ast->left, pipes, in_out);
		status = wait_for_processes();
		if (status != 0)
			return (status);
		if (res == 1)
			return (res);
		execute_ast(minishell, ast->right, pipes, in_out);
	}
	else if (ast->type == AST_OR)
	{
		res = execute_ast(minishell, ast->left, pipes, in_out);
		status = wait_for_processes();
		if (status == 0)
			return (status); //TODO: check if this is the correct behavior, should it return 0 or res?
		if (res == 1)
			return (res);
		execute_ast(minishell, ast->right, pipes, in_out);
	}
	else if (ast->type == AST_SEQUENCE)
	{
	}
	else if (ast->type == AST_SUBSHELL)
	{
	}
	else if (ast->type == AST_REDIR_IN)
	{
		if (ast->right->value[0] == NULL)
		{
			ft_putstr_fd("Error: no file specified\n", STDERR_FILENO);
			return (1);
		}
		in_out[0] = open(ast->right->value[0], O_RDONLY);
		if (in_out[0] == -1)
		{
			ft_putstr_fd("Error: open failed\n", STDERR_FILENO);
			return (1);
		}
		execute_ast(minishell, ast->left, pipes, in_out);
		close(in_out[0]);
	}
	else if (ast->type == AST_REDIR_OUT)
	{
		if (ast->right->value[0] == NULL)
		{
			ft_putstr_fd("Error: no file specified\n", STDERR_FILENO);
			return (1);
		}
		if (access(ast->right->value[0], W_OK) != 0)
		{
			ft_fprintf(STDERR_FILENO, "minishell: %s: Permission denied\n", ast->right->value[0]);
			return (1); //TODO: dont stop the all execution
		}
		execute_ast(minishell, ast->left, pipes, in_out);
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
	}
	else if (ast->type == AST_REDIR_OUT_APPEND)
	{
		if (ast->right->value[0] == NULL)
		{
			ft_putstr_fd("Error: no file specified\n", STDERR_FILENO);
			return (1);
		}
		if (access(ast->right->value[0], W_OK) != 0)
		{
			ft_fprintf(STDERR_FILENO, "minishell: %s: Permission denied\n", ast->right->value[0]);
			return (1); //TODO: dont stop the all execution
		}
		execute_ast(minishell, ast->left, pipes, in_out);
		file_fd = open(ast->right->value[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (file_fd == -1)
		{
			ft_putstr_fd("Error: open failed\n", STDERR_FILENO);
			return (1); //TODO: dont stop the all execution
		}
		copy_fd_contents(in_out[0], file_fd);
		close(file_fd);
		close_fds(in_out, pipes);
		wait_for_processes();
	}
	else if (ast->type == AST_HEREDOC)
	{
	}
	else if (ast->type == AST_VARIABLE)
	{
	}
	//execute the node corresponding to type
	return (0);
}

/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return 0 on success -1 on exit request
 */
int	execute_input(t_minishell *minishell, char *input)
{
	t_ast_node	*ast;
	int 		in_out[3];
	int 		pipes[2];

	in_out[0] = STDIN_FILENO;
	in_out[1] = STDOUT_FILENO;
	in_out[2] = -1;
	pipes[0] = -1;
	pipes[1] = -1;
	ast = parse_input(minishell, input);
	if (!ast)
		return (0);
	minishell->ast = ast;

	disable_termios(minishell->term);

	execute_ast(minishell, ast, pipes, in_out);
	minishell->exit_code = wait_for_processes();
	enable_termios(minishell->term);

	ft_fprintf(STDERR_FILENO, "final exit code = %d\n", minishell->exit_code);
	free_ast(ast);
	minishell->ast = NULL;
	return (minishell->exit_code);
}
