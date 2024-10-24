/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_output.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 00:14:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/13 00:14:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Verify file access and print error message if needed
 *
 * @param char *file The file to check
 * @return int 1 on success, 0 on failure
 */
static int	test_file_access(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		if (access(file, F_OK) == 0)
		{
			if (access(file, R_OK) == -1)
				ft_fprintf(STDERR_FILENO,
					"minishell: %s: No permissions\n", file);
			else
				ft_fprintf(STDERR_FILENO,
					"minishell: %s: Error opening file\n", file);
		}
		else
			return (1);
		return (0);
	}
	close(fd);
	return (1);
}

/**
 * @brief Execute the ast before redirecting the output to a file
 * 				and check if the file is accessible before doing so
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, -1 on failure
 */
static int	redirect_output(t_minishell *minishell, t_ast_node *ast,
						int *pipes, int *in_out)
{
	int	i;

	if (!verify_redirection(minishell, ast))
	{
		minishell->exit_code = 1;
		return (1);
	}
	i = 1;
	while (ast->right->value[i])
		ft_tabadd(&ast->left->value, ast->right->value[i++]);
	execute_ast(minishell, ast->left, pipes, in_out);
	return (0);
}

/**
 * @brief Open a file and print an error message if it fails
 *
 * @param char *file The file to open
 * @param int flags The flags to open the file with
 * @return int The file descriptor on success, -1 on failure
 */
int	open_file(char *file, int flags)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: No such file or directory\n",
			file);
		return (-1);
	}
	return (fd);
}

/**
 * @brief Redirect the output of the command to a file
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, -1 on failure
 */
int	execute_redirect_output(t_minishell *minishell, t_ast_node *ast,
						int *pipes, int *in_out)
{
	int	file_fd;

	if (!test_file_access(ast->right->value[0]))
		return (0);
	if (redirect_output(minishell, ast, pipes, in_out))
		return (0);
	if (minishell->exit_code != 0)
		return (0);
	file_fd = open_file(ast->right->value[0], O_WRONLY | O_CREAT | O_TRUNC);
	if (file_fd == -1)
		return (0);
	if (!ast->is_last)
	{
		close(file_fd);
		file_fd = STDOUT_FILENO;
		minishell->exit_code = 0;
		return (1);
	}
	if (in_out[0] != STDIN_FILENO)
	{
		copy_fd_contents(in_out[0], file_fd);
		close_fds(in_out, pipes);
	}
	minishell->exit_code = 0;
	return (0);
}

/**
 * @brief Redirect the output of the command to a file
 * 			by appending the output to the file
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @param int append
 * @return int 0 on success, -1 on failure
 */
int	execute_redirect_output_append(t_minishell *minishell, t_ast_node *ast,
								int *pipes, int *in_out)
{
	int	file_fd;

	if (!test_file_access(ast->right->value[0]))
		return (0);
	if (redirect_output(minishell, ast, pipes, in_out))
		return (0);
	if (minishell->exit_code != 0)
		return (0);
	file_fd = open_file(ast->right->value[0], O_WRONLY | O_CREAT | O_APPEND);
	if (file_fd == -1)
		return (0);
	if (!ast->is_last)
	{
		close(file_fd);
		file_fd = STDOUT_FILENO;
		minishell->exit_code = 0;
		return (1);
	}
	if (in_out[0] != STDIN_FILENO)
	{
		copy_fd_contents(in_out[0], file_fd);
		close_fds(in_out, pipes);
	}
	minishell->exit_code = 0;
	return (0);
}
