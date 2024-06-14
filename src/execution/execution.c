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
 * @brief Execute custom command before parsing the input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return int 1 if a command was executed, 0 otherwise,
 * 			2 if exit was called
 */
static int	execute_custom_command(t_minishell *minishell, char *input)
{
	if (ft_strncmp(input, "exit", 4) == 0)
		return (-1);
	else if (ft_strcmp(input, "history") == 0)
	{
		history_print(minishell);
		return (1);
	}
	return (0);
}

/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char **args
 * @param char **env
 * @return int 1 on success, 0 on failure
 */
static int	execute_command(char *path, char **args, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execve(path, args, env);
		ft_putstr_fd("Error: execve failed\n", 2);
		return (0);
	}
	else if (pid < 0)
	{
		ft_putstr_fd("Error: fork failed\n", 2);
		return (0);
	}
	waitpid(pid, &status, 0);
	return (1);
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
	char	*path;
	t_ast	*tmp;
	char	**args;

    args = ft_calloc(1, sizeof(char *));
    if (args == NULL)
		return (0);
	args[0] = NULL;
	path = NULL;
	tmp = ast;
	while (tmp)
	{
		if (tmp->type == FULL_COMMAND)
			execute_ast(minishell, tmp->children);
		else if (tmp->type == COMMAND)
		{
			// TODO: create a command to read ast and return infos
			path = get_path(tmp->value, minishell->env);
			if (path)
			{
				//set empty flags for now
				if (!execute_command(path, args, minishell->env))
				{
					free(path);
					return (0);
				}
				free(path);
			}
			else
				ft_putstr_fd("Error: command not found\n", 2);
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
