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

static int execute_ast(t_minishell *minishell, t_ast_node *ast);

/**
 * @brief Check the following things:
 * 	- If the input is empty, print a new line and return NULL
 * 	- Trim the input
 * 	- Add the input to the history
 * 	- Print the input in debug mode if needed
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return char* the trimmed input
 */
static char	*check_input(t_minishell *minishell, char *input)
{
	char	*trimmed;

	if (ft_strlen(input) == 0)
	{
		terminal_print("", 1, STDOUT_FILENO);
		free(input);
		return (NULL);
	}
	trimmed = ft_strtrim(input, WHITESPACES);
	free(input);
	if (!trimmed)
		return (NULL);
	debug_execution(trimmed);
	if (ft_isprint(*trimmed))
		history_add(minishell, trimmed, 1);
	return (trimmed);
}

static int execute_command(t_minishell *minishell, t_ast_node *ast) {
	if (strcmp(ast->value[0], "exit") == 0) {
		return 1;  // Signal to exit minishell
	}

	pid_t pid = fork();
	if (pid == 0) {
		// Child process
		if (execvp(ast->value[0], ast->value) == -1) {
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	} else if (pid > 0) {
		// Parent process
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)) {
			minishell->exit_code = WEXITSTATUS(status);
		}
	} else {
		// Fork failed
		perror("fork");
		minishell->exit_code = 1;
	}

	return 0;
}

static int execute_pipe(t_minishell *minishell, t_ast_node *ast) {
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("pipe");
		minishell->exit_code = 1;
		return 0;
	}

	pid_t pid_left = fork();
	if (pid_left == 0) {
		// Left command: redirect stdout to pipe
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		execute_ast(minishell, ast->left);
		exit(minishell->exit_code);
	}

	pid_t pid_right = fork();
	if (pid_right == 0) {
		// Right command: redirect stdin from pipe
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		execute_ast(minishell, ast->right);
		exit(minishell->exit_code);
	}

	// Parent process closes both ends of the pipe
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, NULL, 0);

	return 0;
}

static int execute_and(t_minishell *minishell, t_ast_node *ast) {
	execute_ast(minishell, ast->left);
	if (minishell->exit_code == 0) {
		execute_ast(minishell, ast->right);
	}
	return 0;
}

static int execute_or(t_minishell *minishell, t_ast_node *ast) {
	execute_ast(minishell, ast->left);
	if (minishell->exit_code != 0) {
		execute_ast(minishell, ast->right);
	}
	return 0;
}

static int execute_redirection(t_minishell *minishell, t_ast_node *ast) {
	int fd;

	if (ast->type == AST_REDIR_OUT) {
		fd = open(ast->right->value[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1) {
			perror("open");
			minishell->exit_code = 1;
			return 0;
		}
		dup2(fd, STDOUT_FILENO);
	} else if (ast->type == AST_REDIR_IN) {
		fd = open(ast->right->value[0], O_RDONLY);
		if (fd == -1) {
			perror("open");
			minishell->exit_code = 1;
			return 0;
		}
		dup2(fd, STDIN_FILENO);
	}

	close(fd);
	execute_ast(minishell, ast->left);  // Execute the command with redirection
	return 0;
}

static int execute_subshell(t_minishell *minishell, t_ast_node *ast) {
	pid_t pid = fork();
	if (pid == 0) {
		// Child process executes the subshell
		execute_ast(minishell, ast->left);
		exit(minishell->exit_code);
	} else {
		// Parent process waits for the subshell to finish
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)) {
			minishell->exit_code = WEXITSTATUS(status);
		}
	}
	return 0;
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
	if (ast == NULL)
		return (0);
	if (ast->type == AST_COMMAND)
		return (execute_command(minishell, ast));
	if (ast->type == AST_PIPE)
		return (execute_pipe(minishell, ast));
	if (ast->type == AST_AND)
		return (execute_and(minishell, ast));
	if (ast->type == AST_OR)
		return (execute_or(minishell, ast));
	if (ast->type == AST_REDIR_OUT || ast->type == AST_REDIR_IN)
		return (execute_redirection(minishell, ast));
	if (ast->type == AST_SUBSHELL)
		return (execute_subshell(minishell, ast));
	return (0);
}

/**
 * @brief Execute the command given in input
 *
 * @param t_minishell *minishell
 * @param char *input
 * @return 0 on success -1 on exit request
 */
int	execute(t_minishell *minishell, char *input)
{
	t_ast_node	*ast;
	int		res;
	char	*trimmed;

	trimmed = check_input(minishell, input);
	if (!trimmed)
		return (0);
	ast = parse_input(minishell, trimmed);
	if (!ast)
	{
		free(trimmed);
		return (0);
	}
	free(trimmed);
	res = execute_ast(minishell, ast);
	free_ast(ast);
	minishell->exit_code = res;
	return (res);
}
