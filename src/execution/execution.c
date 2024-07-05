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
 * @return int 1 on success, 0 on failure
 */
static int	execute_cmd(t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd->cmd_exec, cmd->argv, cmd->env) == -1)
		{
			ft_printf("minishell: command not found: %s\n", cmd->cmd_name);
			cmd->exit_status = 127;
		}
	}
	else if (pid < 0)
	{
		ft_putstr_fd("Error: fork failed\n", 2);
		cmd->exit_status = 1;
	}
	else
	{
		waitpid(pid, &status, 0);
		cmd->exit_status = WEXITSTATUS(status);
	}
	return (1);
}

static int	execute_command(t_minishell *minishell, t_ast *ast)
{
	t_cmd	*cmd;
	int 	res;

	cmd = command_maker(minishell, ast);
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd->cmd_name, "env") == 0)
	{
		command_env(cmd);
		res = 1;
	}
	else if (ft_strcmp(cmd->cmd_name, "exit") == 0)
	{
		command_exit(cmd);
		res = 2;
	}
	else if (ft_strcmp(cmd->cmd_name, "history") == 0)
	{
		command_history(cmd, minishell);
		res = 1;
	}
	else if (ft_strcmp(cmd->cmd_name, "echo") == 0)
	{
		command_echo(cmd);
		res = 1;
	}
	else if (ft_strcmp(cmd->cmd_name, "cd") == 0)
	{
		command_cd(cmd);
		res = 1;
	}
	else
		res = execute_cmd(cmd);
	free_cmd(cmd);
	return (res);
}

/**
 * @brief Execute the ast
 *
 * @param t_minishell *minishell
 * @param t_ast *ast
 * @return int 1 on success, 0 on failure
 */
static int	execute_ast(t_minishell *minishell, t_ast *ast)
{
	t_ast	*tmp;

	tmp = ast;
	while (tmp)
	{
		if (tmp->type == FULL_COMMAND)
		{
			if (execute_ast(minishell, tmp->children))
				return (1);
		}
		else if (tmp->type == COMMAND)
		{
			if (execute_command(minishell, tmp) == 2)
				return (1);
		}
		tmp = tmp->next;
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
	res = execute_ast(minishell, ast);
	free_ast(ast);
	free(input);
	return (res);
}
