#include "minishell.h"

/**
 * @brief Handle signals for heredoc (Ctrl+C)
 *
 * @param int sig The signal number
 */
static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\nminishell: heredoc interrupted by Ctrl+C\n", STDERR_FILENO);
		exit(130);
	}
}

/**
 * @brief Write the heredoc content to the pipe
 *
 * @param t_heredoc_info *heredoc_info Information sur le heredoc
 * @return int 1 on success, 0 on failure
 */
static int	write_heredoc(t_heredoc_info *heredoc_info)
{
	char	*tmp;

	if (heredoc_info->expend_var)
		tmp = replace_variables(heredoc_info->minishell, heredoc_info->line);
	else
		tmp = ft_strdup(heredoc_info->line);
	free(heredoc_info->line);
	if (write(heredoc_info->pipes[1], tmp, ft_strlen(tmp)) == -1)
	{
		ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
		free(tmp);
		close(heredoc_info->pipes[1]);
		return (1);
	}
	free(tmp);
	return (0);
}

/**
 * @brief Load the heredoc info
 *
 * @param t_minishell *minishell
 * @param int *pipes
 * @param int *in_out
 * @param char *delimiter
 * @return t_heredoc_info*
 */
static t_heredoc_info	*load_heredoc_info(t_minishell *minishell, int *pipes, int *in_out, char *delimiter)
{
	t_heredoc_info	*heredoc_info;

	heredoc_info = malloc(sizeof(t_heredoc_info));
	if (!heredoc_info)
		return (NULL);
	heredoc_info->minishell = minishell;
	heredoc_info->delimiter = ft_strdup(delimiter);
	heredoc_info->pipes = pipes;
	heredoc_info->in_out = in_out;
	heredoc_info->line = NULL;
	heredoc_info->expend_var = 1;
	return (heredoc_info);
}

/**
 * @brief Check for heredoc validity before starting the process
 *
 * @param t_ast_node *ast
 * @return int 1 on success, 0 on failure
 */
int	heredoc_valid(t_ast_node *ast)
{
	if (ast->right->value[0] == NULL)
	{
		ft_putstr_fd("Error: no delimiter specified\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

/**
 * @brief Run the heredoc logic in a fork and handle signals in the child
 *
 * @param t_minishell *minishell
 * @param char *delimiter
 * @param int *pipes
 * @param int *in_out
 * @return int 1 on success, 0 on failure
 */
int	run_heredoc(t_minishell *minishell, char *delimiter, int *pipes, int *in_out)
{
	pid_t				pid;
	t_heredoc_info		*heredoc_info;
	char				*tmp;
	int					i;
	struct sigaction	sa;
	int					status;
	struct sigaction	sa_parent;

	if (pipe(pipes) == -1)
	{
		ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
		return (0);
	}
	sigemptyset(&sa_parent.sa_mask);
	sa_parent.sa_flags = 0;
	sa_parent.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_parent, NULL);
	sigaction(SIGQUIT, &sa_parent, NULL);

	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("Error: fork failed\n", STDERR_FILENO);
		return (0);
	}
	else if (pid == 0)
	{
		heredoc_info = load_heredoc_info(minishell, pipes, in_out, delimiter);
		if (!heredoc_info)
			exit(1);
		memset(&sa, 0, sizeof(sa));
		sa.sa_flags = 0;
		sa.sa_handler = heredoc_signal_handler;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
		close(pipes[0]);
		i = 1;
		while (1)
		{
			ft_putstr_fd("> ", STDOUT_FILENO);
			heredoc_info->line = get_next_line(STDIN_FILENO);
			if (!heredoc_info->line)
			{
				ft_fprintf(STDERR_FILENO, "\nminishell: warning: heredoc at line %d delimited by end-of-file (wanted `%s`)\n", i, heredoc_info->delimiter);
				close(heredoc_info->pipes[1]);
				free(heredoc_info->line);


				free(heredoc_info->delimiter);
				free_minishell(heredoc_info->minishell);
				free(heredoc_info);

				exit(0);
			}

			tmp = ft_strdup(heredoc_info->line);
			tmp[ft_strlen(heredoc_info->line) - 1] = '\0';

			if (ft_strcmp(tmp, heredoc_info->delimiter) == 0)
			{
				free(tmp);
				break;
			}
			free(tmp);
			if (write_heredoc(heredoc_info))
			{
				free(heredoc_info->line);
				free(heredoc_info->delimiter);
				close(pipes[1]);
				free_minishell(heredoc_info->minishell);
				free(heredoc_info);
				exit(1);
			}
			i++;
		}
		free(heredoc_info->line);
		free(heredoc_info->delimiter);
		close(pipes[1]);
		free_minishell(heredoc_info->minishell);
		free(heredoc_info);
		exit(0);
	}
	else
	{
		close(pipes[1]);
		waitpid(pid, &status, 0);
		sigaction(SIGINT, &sa_parent, NULL);
		sigaction(SIGQUIT, &sa_parent, NULL);
		if (WIFEXITED(status))
			minishell->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			minishell->exit_code = 130;
		in_out[0] = pipes[0];

		if (minishell->exit_code != 0)
			return (0);
		return (1);
	}
}
