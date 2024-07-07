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
    if (pid < 0)
    {
        ft_putstr_fd("Error: fork failed\n", 2);
        cmd->exit_status = 1;
        return (0);
    }
	else if (pid == 0)
	{
        dup2(cmd->pipe_fd[0], STDIN_FILENO);
        dup2(cmd->pipe_fd[1], STDOUT_FILENO);
        ft_printf("cmd_exec: %s\n", cmd->cmd_exec);
		if (execve(cmd->cmd_exec, cmd->argv, cmd->env) == -1)
		{
			ft_printf("minishell: command not found: %s\n", cmd->cmd_name);
			cmd->exit_status = 127;
		}
        ft_printf("exit\n");
	}
    waitpid(pid, &status, 0);
    cmd->exit_status = WEXITSTATUS(status);
	return (1);
}



static char	*ft_get_path_test(t_cmd *cmd)
{
    int		i;
    char	*path;
    char	**paths;
    char	*tmp;

    if (access(cmd->cmd_name, X_OK) == 0)
        return (cmd->cmd_name);
    i = 0;
    while (ft_strncmp(cmd->env[i], "PATH=", 5))
        i++;
    paths = ft_split(cmd->env[i] + 5, ":");
    i = -1;
    while (paths[++i])
    {
        tmp = ft_strjoin(paths[i], "/");
        path = ft_strjoin(tmp, cmd->cmd_name);
        free(tmp);
        if (access(path, X_OK) == 0)
        {
            ft_freetab(paths);
            return (path);
        }
        free(path);
    }
    ft_freetab(paths);
    close(cmd->pipe_fd[0]);
    close(cmd->pipe_fd[1]);
    if (cmd->fd_to_close != -1)
        close(cmd->fd_to_close);
    exit(1);
}

static void	ft_exec_cmd_test(t_cmd *cmd)
{
    int		pid;
    char	*path;

    pid = fork();
    if (pid < 0)
        return ;
    else if (pid == 0)
    {
        path = ft_get_path_test(cmd);
        dup2(cmd->pipe_fd[0], STDIN_FILENO);
        dup2(cmd->pipe_fd[1], STDOUT_FILENO);
        close(cmd->pipe_fd[0]);
        close(cmd->pipe_fd[1]);
        if (cmd->fd_to_close != -1)
            close(cmd->fd_to_close);
        execve(path, cmd->argv, cmd->env);
        perror("execve");
        free(path);
        exit(1);
    }
    waitpid(pid, NULL, 0);
}

static void	ft_exec_cmd_test_2(t_cmd *cmd)
{
    int		fd[2];

    pipe(fd);
    cmd->pipe_fd[1] = fd[1];
    cmd->fd_to_close = fd[0];
    ft_exec_cmd_test(cmd);
    close(fd[1]);
    close(cmd->pipe_fd[0]);
    close(cmd->pipe_fd[0]);
    cmd->pipe_fd[0] = fd[0];

    close(cmd->pipe_fd[0]);
    close(cmd->pipe_fd[1]);
    close(fd[0]);
    close(fd[1]);
}

static int	execute_command(t_minishell *minishell, t_cmd *cmd)
{
	int 	res;

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
    {
        //res = execute_cmd(cmd);
        ft_exec_cmd_test_2(cmd);
        res = 1;
    }
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
static int	execute_ast(t_minishell *minishell, t_ast *ast, int pipe_fd[2], int fd_to_close)
{
	t_ast	*tmp;
    t_cmd	*cmd;

	tmp = ast;
	while (tmp)
	{
        ft_printf("type: %d\n", tmp->type);
		if (tmp->type == FULL_COMMAND)
		{
			if (execute_ast(minishell, tmp->children, pipe_fd, fd_to_close) == 2)
				return (1);
		}
		else if (tmp->type == COMMAND)
		{
            cmd = command_maker(minishell, tmp, pipe_fd, fd_to_close);
			if (execute_command(minishell, cmd) == 2)
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
	res = execute_ast(minishell, ast, pipe_fd, -1);
	free_ast(ast);
	free(input);
	return (res);
}
