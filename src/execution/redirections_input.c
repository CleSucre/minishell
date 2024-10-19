/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_input.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 19:34:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/05 19:34:00 by julthoma         ###   ########.fr       */
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
				ft_fprintf(STDERR_FILENO, "minishell: %s: No permissions\n", file);
			else
				ft_fprintf(STDERR_FILENO, "minishell: %s: Error opening file\n", file);
		}
		else
			ft_fprintf(STDERR_FILENO, "minishell: %s: No such file or directory\n", file);
		return (0);
	}
	close(fd);
	return (1);
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
int	execute_redirect_input(t_minishell *minishell, t_ast_node *ast,
					int *pipes, int *in_out)
{
	if (!verify_redirection(minishell, ast))
		return (0);
	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no file specified\n", STDERR_FILENO);
		return (0);
	}
	if (!test_file_access(ast->right->value[0]))
		return (0);
	in_out[0] = open(ast->right->value[0], O_RDONLY);
	setup_pipes(pipes, in_out, 0);
	close(pipes[0]);
	close(pipes[1]);
	return (execute_ast(minishell, ast->left, pipes, in_out));
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
int	execute_heredoc(t_minishell *minishell, t_ast_node *ast,
				int *pipes, int *in_out)
{
	int		status;

	status = heredoc_valid(ast, pipes);
	if (status <= 0)
		return (status);
	status = run_heredoc(minishell, ast->right->value[0], pipes, in_out);
	if (!status)
		return (-1);
	if (!ast->is_last || !ast->left)
	{
		close_fds(in_out, pipes);
		in_out[0] = STDIN_FILENO;
		in_out[1] = STDOUT_FILENO;
		in_out[2] = -1;
	}
	return (execute_ast(minishell, ast->left, pipes, in_out));
}
