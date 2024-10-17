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
	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no file specified\n", STDERR_FILENO);
		return (1);
	}
	in_out[0] = open(ast->right->value[0], O_RDONLY);
	setup_pipes(pipes, in_out, 0);
	if (in_out[0] == -1)
	{
		ft_putstr_fd("Error: open failed\n", STDERR_FILENO);
		return (1);
	}
	close(pipes[0]);
	close(pipes[1]);
	return (execute_ast(minishell, ast->left, pipes, in_out));
}

/**
 * @brief Check if the given input is valid before starting the heredoc
 *
 * @param int *in_out
 * @param int *pipes
 * @return int 1 on valid, 0 on invalid, -1 on failure
 */
static int	heredoc_valid(t_ast_node *ast, int *pipes)
{
	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no delimiter specified\n", STDERR_FILENO);
		return (0);
	}
	if (pipe(pipes) == -1)
	{
		ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
		return (-1);
	}
	return (1);
}

/**
 * @brief Run the heredoc logic
 *
 * @param t_minishell *minishell
 * @param char *delimiter
 * @param int *pipes
 * @param int *in_out
 * @return int 1 on success, 0 on failure
 */
static int	run_heredoc(t_minishell *minishell, char *delimiter,
					int *pipes, int *in_out)
{
	char	*tmp;
	char	*line;

	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
			break ;
		tmp = replace_variables(minishell, line);
		free(line);
		if (write(pipes[1], tmp, strlen(tmp)) == -1)
		{
			ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
			free(tmp);
			close_fds(in_out, pipes);
			return (0);
		}
		free(tmp);
	}
	free(line);
	in_out[0] = pipes[0];
	close(pipes[1]);
	return (1);
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
	return (execute_ast(minishell, ast->left, pipes, in_out));
}
