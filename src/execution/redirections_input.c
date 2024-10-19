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
	if (!verify_redirection(minishell, ast))
		return (0);
	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no file specified\n", STDERR_FILENO);
		return (0);
	}
	in_out[0] = open(ast->right->value[0], O_RDONLY);
	setup_pipes(pipes, in_out, 0);
	if (in_out[0] == -1)
	{
		ft_putstr_fd("Error: open failed\n", STDERR_FILENO);
		return (0);
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
	if (pipe(pipes) == -1 && !ast->is_last)
	{
		ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
		return (-1);
	}
	return (1);
}

/**
 * @brief Write the heredoc content to the pipe
 *
 * @param t_minishell *minishell
 * @param int *pipes
 * @param int *in_out
 * @param char *line
 * @return int 1 on success, 0 on failure
 */
static int	write_heredoc(t_minishell *minishell, int *pipes,
						int *in_out, char *line)
{
	char	*tmp;

	tmp = replace_variables(minishell, line);
	free(line);
	if (write(pipes[1], tmp, ft_strlen(tmp)) == -1)
	{
		ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
		free(tmp);
		close_fds(in_out, pipes);
		return (1);
	}
	free(tmp);
	return (0);
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
		if (!line)
		{
			ft_fprintf(STDERR_FILENO, "Error: get_next_line failed\n");
			close_fds(in_out, pipes);
			free(line);
			return (1);
		}
		tmp = ft_strdup(line);
		tmp[ft_strlen(line) - 1] = '\0';
		if (!line || ft_strcmp(tmp, delimiter) == 0)
		{
			free(tmp);
			break ;
		}
		free(tmp);
		if (write_heredoc(minishell, pipes, in_out, line))
			return (0);
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
	if (!ast->is_last || !ast->left)
	{
		close_fds(in_out, pipes);
		in_out[0] = STDIN_FILENO;
		in_out[1] = STDOUT_FILENO;
		in_out[2] = -1;
	}
	return (execute_ast(minishell, ast->left, pipes, in_out));
}
