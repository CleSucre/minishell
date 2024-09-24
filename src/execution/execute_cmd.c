/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: votre_nom <votre_email@example.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 18:13:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/23 12:00:00 by votre_nom        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/**
 * @brief Exécute une commande externe en utilisant fork et execve
 */
static int	execute_external(t_minishell *minishell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (errno);
	}
	else if (pid == 0)
	{
		// Processus enfant
		// Redirection des entrées/sorties si nécessaire
		if (cmd->input_fd != STDIN_FILENO)
		{
			dup2(cmd->input_fd, STDIN_FILENO);
			close(cmd->input_fd);
		}
		if (cmd->output_fd != STDOUT_FILENO)
		{
			dup2(cmd->output_fd, STDOUT_FILENO);
			close(cmd->output_fd);
		}
		if (cmd->to_close != -1)
			close(cmd->to_close);
		execute_path(cmd);
		destroy_cmd(cmd);
		free_minishell(minishell);
		// Si execve échoue
		perror("execve");
		exit(errno);
	}
	else
	{
		// Processus parent
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	return (0);
}

/**
 * @brief Fonction principale pour exécuter une commande
 */
int	execute_cmd(t_minishell *minishell, t_ast_node *ast, int in_out[3])
{
	t_cmd	*cmd;
	int		res;

	cmd = create_cmd(ast, minishell->env, in_out);
	if (!cmd)
		return (1);
	if (is_builtin_command(cmd))
		res = execute_builtin_command(minishell, cmd);
	else
		res = execute_external(minishell, cmd);
	destroy_cmd(cmd);
	return (res);
}
