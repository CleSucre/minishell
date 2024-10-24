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
 * @brief Execute the ast respecting subshell logic
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, 1 on exit request
 */
int	execute_subshell(t_minishell *minishell, t_ast_node *ast,
					int *pipes, int *in_out)
{
	int		res;
	char	**tmp_env;
	char	**old_env;

	tmp_env = ft_tabdup((const char **)minishell->env);
	if (!tmp_env)
	{
		perror("Error: ft_tabdup failed");
		return (-1);
	}
	old_env = minishell->env;
	minishell->env = tmp_env;
	res = execute_ast(minishell, ast->left, pipes, in_out);
	minishell->env = old_env;
	ft_tabfree(tmp_env);
	reload_env(minishell->env);
	if (res != 0)
		return (res);
	wait_for_processes();
	return (res);
}

/**
 * @brief Execute the ast, minishell->exit_code will be set to
 * 			the return value of the last command
 * 		Always start by commands on the left, then the right
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @return 0 on success, 1 on exit request
 */
int	execute_ast(t_minishell *minishell, t_ast_node *ast,
				int *pipes, int *in_out)
{
	if (!ast)
		return (0);
	if (ast->type == AST_COMMAND)
		return (execute_cmd(minishell, ast, pipes, in_out));
	else if (ast->type == AST_PIPE)
		return (execute_pipe(minishell, ast, pipes, in_out));
	else if (ast->type == AST_AND)
		return (execute_and(minishell, ast, pipes, in_out));
	else if (ast->type == AST_OR)
		return (execute_or(minishell, ast, pipes, in_out));
	else if (ast->type == AST_SUBSHELL)
		return (execute_subshell(minishell, ast, pipes, in_out));
	else if (ast->type == AST_REDIR_IN)
		return (execute_redirect_input(minishell, ast, pipes, in_out));
	else if (ast->type == AST_HEREDOC)
		return (execute_heredoc(minishell, ast, pipes, in_out));
	else if (ast->type == AST_REDIR_OUT)
		return (execute_redirect_output(minishell, ast, pipes, in_out));
	else if (ast->type == AST_REDIR_OUT_APPEND)
		return (execute_redirect_output_append(minishell, ast, pipes, in_out));
	return (0);
}

/**
 * @brief Execute all the heredoc nodes in the ast
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @return int 1 on success, 0 on failure
 */
int	pre_execute_ast(t_minishell *minishell, t_ast_node *ast)
{
	int		res;

	if (!ast)
		return (1);
	if (ast->type == AST_HEREDOC)
	{
		res = pre_execute_heredoc(minishell, ast);
		add_fd_to_close(minishell->opened_fds, ast->heredoc_fd);
		if (res == 0)
			return (res);
	}
	res = pre_execute_ast(minishell, ast->left);
	if (res == 0)
		return (res);
	res = pre_execute_ast(minishell, ast->right);
	return (res);
}

static void	init_in_out_pipe(int *in_out, int *pipes)
{
	in_out[0] = STDIN_FILENO;
	in_out[1] = STDOUT_FILENO;
	in_out[2] = -1;
	pipes[0] = -1;
	pipes[1] = -1;
}

/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return 0 on success 1 on exit request
 */
int	execute_input(t_minishell *minishell, char *input)
{
	t_ast_node	*ast;
	int			in_out[3];
	int			pipes[2];
	int			res;

	init_in_out_pipe(in_out, pipes);
	ast = parse_input(minishell, input);
	if (!ast)
		return (0);
	minishell->ast = ast;
	disable_termios(minishell->term);
	if (count_heredoc(ast) >= MAX_HEREDOC)
	{
		exceed_max_heredoc(minishell, ast);
		return (1);
	}
	res = pre_execute_ast(minishell, ast);
	if (res == 0)
		return (pre_executed_failed(minishell, ast));
	res = execute_ast(minishell, ast, pipes, in_out);
	close_all_fds(minishell->opened_fds);
	enable_termios(minishell->term);
	free_ast(ast);
	minishell->ast = NULL;
	return (res);
}
