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
 * @brief Execute the ast respecting AND operator logic
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, 1 on exit request
 */
int execute_and(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out)
{
	int res;
	int status;

	res = execute_ast(minishell, ast->left, pipes, in_out);
	status = wait_for_processes();
	if (status != 0)
		return (status);
	if (res == 1)
		return (res);
	execute_ast(minishell, ast->right, pipes, in_out);
	return (0);
}

/**
 * @brief Execute the ast respecting OR operator logic
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, 1 on exit request
 */
int execute_or(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out)
{
	int res;
	int status;

	res = execute_ast(minishell, ast->left, pipes, in_out);
	status = wait_for_processes();
	if (status == 0)
		return (status); //TODO: check if this is the correct behavior, should it return 0 or res?
	if (res == 1)
		return (res);
	execute_ast(minishell, ast->right, pipes, in_out);
	return (0);
}

/**
 * @brief Execute the ast respecting subshell logic
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @param int *pipes
 * @param int *in_out
 * @return int 0 on success, 1 on exit request
 */
int execute_subshell(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out)
{
	int res;
	char **tmp_env;
	char **old_env;

	tmp_env = ft_tabdup((const char **)minishell->env);
	if (!tmp_env) {
		perror("Error: ft_tabdup failed");
		return (-1);
	}
	old_env = minishell->env;
	minishell->env = tmp_env;
	res = execute_ast(minishell, ast->left, pipes, in_out);
	minishell->env = old_env;
	ft_tabfree(tmp_env);
	reload_env(minishell->env);
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
 * @return Last command exit code
 */
int	execute_ast(t_minishell *minishell, t_ast_node *ast, int *pipes, int *in_out)
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
		return (execute_redirect_output(minishell, ast, pipes, in_out, 0));
	else if (ast->type == AST_REDIR_OUT_APPEND)
		return (execute_redirect_output(minishell, ast, pipes, in_out, 1));
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
	enable_termios(minishell->term);

	free_ast(ast);
	minishell->ast = NULL;
	return (minishell->exit_code);
}
