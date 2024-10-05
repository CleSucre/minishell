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
}

/**
 * @brief Setup pipes according to the command
 * 			if its a redirection or a simple pipe for the next cmd
 *
 * @param int *pipes pipe[0] = read, pipe[1] = write
 * @param int *in_out in_out[0] = read, in_out[1] = write, in_out[2] = to_close
 * @return int 0 on success, -1 on failure
 */
static int setup_pipes(int *pipes, int *in_out, int final_out)
{
	if (!final_out)
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
 * @brief Execute the ast, minishell->exit_code will be set to
 * 			the return value of the last command
 * 		Always start by commands on the left, then the right
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @return int return 1 if the command is an exit request, 0 otherwise
 */
static int	execute_ast(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out)
{
	int	res;

	if (!ast)
		return (0);
	if (ast->type == AST_COMMAND)
	{
		if (setup_pipes(pipes, in_out, ast->is_last) == -1)
			return (1);
		minishell->exit_code = execute_cmd(minishell, ast, in_out);
		ft_fprintf(STDERR_FILENO, "exit code = %d\n", minishell->exit_code);
		if (minishell->exit_code < 0)
			return (minishell->exit_code);
		close_fds(in_out, pipes);
		in_out[0] = pipes[0];
	}
	else if (ast->type == AST_PIPE)
	{
		res = execute_ast(minishell, ast->left, pipes, in_out);
		if (res == 1)
			return (res);
		res = execute_ast(minishell, ast->right, pipes, in_out);
		if (res == 1)
			return (res);
	}
	else if (ast->type == AST_AND)
	{
		res = execute_ast(minishell, ast->left, pipes, in_out);
		if (res == 1)
			return (res);
		execute_ast(minishell, ast->right, pipes, in_out);
	}
	else if (ast->type == AST_OR)
	{
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
		in_out[1] = open(ast->right->value[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (in_out[1] == -1)
		{
			ft_putstr_fd("Error: open failed\n", STDERR_FILENO);
			return (1);
		}
		execute_ast(minishell, ast->left, pipes, in_out);
		close(in_out[1]);
	}
	else if (ast->type == AST_REDIR_OUT_APPEND)
	{
		//TODO: find a way to append to the file
	}
	else if (ast->type == AST_ASSIGNMENT)
	{
		//TODO: call the function to set the variable in the path
		return (0);
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
	int 		status;
	int			res;
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

	res = execute_ast(minishell, ast, pipes, in_out);

	status = 0;
	status = wait_for_processes(&status);

	enable_termios(minishell->term);

	ft_fprintf(STDERR_FILENO, "final exit code = %d\n", res);
	free_ast(ast);
	minishell->ast = NULL;
	minishell->exit_code = res;
	return (res);
}
