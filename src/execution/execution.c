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

static int	execute_external(t_minishell *minishell, t_cmd *cmd)
{
	(void)minishell;
	(void)cmd;
	return (0);
}

/**
 * @brief Main function for executing a command
 */
int	execute_cmd(t_minishell *minishell, t_ast_node *ast, int input_fd, int output_fd)
{
	t_cmd	*cmd;
	int		res;

	cmd = create_cmd(ast, minishell->env, input_fd, output_fd);
	if (!cmd)
		return (1);
	if (is_builtin_command(cmd))
		res = execute_builtin_command(minishell, cmd);
	else
		res = execute_external(minishell, cmd);
	destroy_cmd(cmd);
	return (res);
}

/**
 * @brief Execute the ast, minishell->exit_code will be set to
 * 			the return value of the last command
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @return int return 1 if the command is an exit request, 0 otherwise
 */
static int	execute_ast(t_minishell *minishell, t_ast_node *ast)
{
	int input_fd;
	int output_fd;

	input_fd = STDIN_FILENO;
	output_fd = STDOUT_FILENO;
	if (ast == NULL)
		return (0);
	if (ast->type == AST_COMMAND)
		return (execute_cmd(minishell, ast, input_fd, output_fd));
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
	int		res;

	ast = parse_input(minishell, input);
	if (!ast)
		return (0);
	disable_termios(minishell->term);
	res = execute_ast(minishell, ast);
	enable_termios(minishell->term);
	free_ast(ast);
	minishell->exit_code = res;
	return (res);
}
