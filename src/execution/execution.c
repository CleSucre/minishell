/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Execute the command given in input
 *
 * @param t_cmd *cmd
 * @return int 0 on success, 1 on failure
 */
static int	execute_path(t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("Error: fork failed\n", STDERR_FILENO);
		cmd->exit_status = 1;
		return (0);
	}
	else if (pid == 0)
	{
		dup2(cmd->input, STDIN_FILENO);
		dup2(cmd->output, STDOUT_FILENO);
		if (cmd->input != STDIN_FILENO)
			close(cmd->input);
		if (cmd->output != STDOUT_FILENO)
			close(cmd->output);
		if (cmd->fd_to_close != -1)
			close(cmd->fd_to_close);
		if (execve(cmd->cmd_exec, cmd->argv, cmd->env) == -1)
		{
			ft_fprintf(STDERR_FILENO, "minishell: command not found: %s\n", cmd->cmd_name);
			cmd->exit_status = 127;
			return (1);
		}
	}
	waitpid(pid, &status, 0);
	cmd->exit_status = WEXITSTATUS(status);
	return (0);
}

static int	execute_cmd(t_minishell *minishell, t_cmd *cmd)
{
	int 	custom_cmd_res;

	if (!cmd)
		return (0);
	custom_cmd_res = execute_custom_command(minishell, cmd);
	if (custom_cmd_res)
		return (custom_cmd_res);
	execute_path(cmd);
	return (1);
}

/**
 * @brief Execute the ast
 *
 * @param t_minishell *minishell
 * @param t_ast *ast
 * @param int *pipe_fd
 * @param int fd_to_close
 * @return int 1 on success, 0 on failure
 */
static int	execute_cmds(t_minishell *minishell, t_ast *ast, int *pipe_fd)
{
	t_cmd	*cmd;
	int 	i;
	int 	fd[2];
	int 	len;

	if (!ast)
		return (0);
	len = ast_count_type(ast, FULL_COMMAND);
	i = 0;
	while (ast)
	{
		if (ast->type == FULL_COMMAND)
		{
			if (i < len - 1)
			{
				pipe(fd);
				pipe_fd[1] = fd[1];
				cmd = load_command(minishell, ast->children, pipe_fd, fd[0]);
				if (cmd)
				{
					execute_cmd(minishell, cmd);
					free_cmd(cmd);
					close(fd[1]);
				}
				if (pipe_fd[0] != STDIN_FILENO)
					close(pipe_fd[0]);
				pipe_fd[0] = fd[0];
			} else {
				pipe_fd[1] = STDOUT_FILENO;
				if (len > 1)
					cmd = load_command(minishell, ast->children, pipe_fd, fd[1]);
				else
					cmd = load_command(minishell, ast->children, pipe_fd, -1);
				if (cmd)
				{
					execute_cmd(minishell, cmd);
					free_cmd(cmd);
				}
				if (len > 1)
				{
					close(fd[0]);
					close(fd[1]);
				}
				if (pipe_fd[0] != STDIN_FILENO)
					close(pipe_fd[0]);
				if (pipe_fd[1] != STDOUT_FILENO)
					close(pipe_fd[1]);
			}
			i++;
		}
		ast = ast->next;
	}
	return (0);
}

/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return int 1 on success, 0 on failure
 */
int	execute(t_minishell *minishell, char *input)
{
	t_ast	*ast;
	int		res;
    int     pipe_fd[2];

	if (ft_strlen(input) == 0)
		return (0);
	input = ft_strtrim(input, WHITESPACES);
	if (!input)
		return (0);
	debug_execution(input);
	if (ft_isprint(*input))
		history_add(minishell, input, 1);
	ast = parse_input(minishell, input);
	if (!ast)
	{
		free(input);
		return (0);
	}
    pipe_fd[0] = STDIN_FILENO;
    pipe_fd[1] = STDOUT_FILENO;
	res = execute_cmds(minishell, ast, pipe_fd);
	free_ast(ast);
	free(input);
	return (res);
}
