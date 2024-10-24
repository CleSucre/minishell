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
				ft_fprintf(STDERR_FILENO,
					"minishell: %s: No permissions\n", file);
			else
				ft_fprintf(STDERR_FILENO,
					"minishell: %s: Error opening file\n", file);
		}
		else
			ft_fprintf(STDERR_FILENO,
				"minishell: %s: No such file or directory\n", file);
		return (0);
	}
	close(fd);
	return (1);
}

/**
 * @brief Find the next AST_COMMAND node and update its value
 *
 * @param char *new_command
 * @param t_ast_node *ast
 */
static int	update_next_command(char * new_command, t_ast_node *ast)
{
	if (ast->type == AST_COMMAND)
	{
		ft_fprintf(STDOUT_FILENO, "ast->value[0]: %s\n", ast->value[0]);
		ft_tabfree(ast->value);
		ast->value = ft_tabnew(1);
		ast->value[0] = ft_strdup(new_command);
		ft_fprintf(STDOUT_FILENO, "ast->value[0]: %s\n", ast->value[0]);
		return (1);
	}
	else
	{
		if (ast->left && update_next_command(new_command, ast->left))
			return (1);
		if (ast->right && update_next_command(new_command, ast->right))
			return (1);
	}
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
int	execute_redirect_input(t_minishell *minishell, t_ast_node *ast,
					int *pipes, int *in_out)
{
	if (!ast->is_last || !ast->left)
		close_fds(in_out, pipes);
	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no file specified\n", STDERR_FILENO);
		return (0);
	}
	ft_fprintf(STDOUT_FILENO, "Redirect input 1\n");
	if (test_file_access(ast->right->value[0]))
	{
		in_out[0] = open(ast->right->value[0], O_RDONLY);
		setup_pipes(pipes, in_out, 0);
		close(pipes[0]);
		close(pipes[1]);
		minishell->exit_code = 0;
		execute_ast(minishell, ast->left, pipes, in_out);
		return (0);
	}
	update_next_command("false", ast);
	execute_ast(minishell, ast->left, pipes, in_out);
	minishell->exit_code = 1;
	return (0);
}

/**
 * @brief Check for heredoc validity before starting the process
 *
 * @param t_ast_node *ast
 * @return int 1 on success, 0 on failure
 */
int	heredoc_valid(t_ast_node *ast)
{
	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no delimiter specified\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	pre_execute_heredoc(t_minishell *minishell, t_ast_node *ast)
{
	if (heredoc_valid(ast) == 0)
		return (0);
	ft_tabdel_empty(ast->right->value);
	return (run_heredoc(minishell, ast->right->value[0], &ast->heredoc_fd));
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
	int	res;

	if (!ast->is_last || !ast->left)
		close_fds(in_out, pipes);
	in_out[0] = ast->heredoc_fd;
	in_out[1] = STDOUT_FILENO;
	in_out[2] = -1;
	res = execute_ast(minishell, ast->left, pipes, in_out);
	return (res);
}
