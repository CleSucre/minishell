#include "minishell.h"

int *g_exit_code;

static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\nminishell: heredoc interrupted by Ctrl+C\n", STDERR_FILENO);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
}

/**
 * @brief Handle the signal in the parent process
 * 			to set the exit code to 130 on SIGINT
 *
 * @param int sig
 */
static void parent_signal_handler(int sig)
{
	if (sig == SIGINT)
		*g_exit_code = 130;
}

/**
 * @brief Write the heredoc content to the pipe
 *
 * @param t_heredoc_info *heredoc_info Information sur le heredoc
 * @return int 1 on success, 0 on failure
 */
static int	write_heredoc(t_heredoc_info *heredoc_info)
{
	int		i;
	ssize_t	bites;

	i = 0;
	while (heredoc_info->texts[i])
	{
		if (ft_strlen(heredoc_info->texts[i]) == 0)
		{
			i++;
			continue ;
		}
		bites = write(heredoc_info->pipes[1], heredoc_info->texts[i], ft_strlen(heredoc_info->texts[i]));
		if (bites == -1)
		{
			ft_fprintf(STDERR_FILENO, "Error 1 on fd %d: write failed\n", heredoc_info->pipes[1]);
			ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
			return (0);
		}
		bites = write(heredoc_info->pipes[1], "\n", 1);
		if (bites == -1)
		{
			ft_fprintf(STDERR_FILENO, "Error 2 on fd %d: write failed\n", heredoc_info->pipes[1]);
			ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * @brief Load the heredoc info
 *
 * @param t_minishell *minishell
 * @param int *pipes
 * @param char *delimiter
 * @return t_heredoc_info*
 */
static t_heredoc_info	*load_heredoc_info(t_minishell *minishell, int *pipes, char *delimiter)
{
	t_heredoc_info	*heredoc_info;

	heredoc_info = malloc(sizeof(t_heredoc_info));
	if (!heredoc_info)
		return (NULL);
	heredoc_info->minishell = minishell;
	heredoc_info->delimiter = ft_strdup(delimiter);
	heredoc_info->pipes = pipes;
	heredoc_info->texts = ft_tabnew(0);
	if (!heredoc_info->texts)
	{
		free(heredoc_info->delimiter);
		free(heredoc_info);
		return (NULL);
	}
	heredoc_info->expend_var = 1;
	return (heredoc_info);
}

static int	read_heredoc(t_heredoc_info *heredoc_info)
{
	int		i;
	char 	*tmp;
	char 	*line;

	i = 0;
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		tmp = get_next_line(STDIN_FILENO);
		line = ft_strtrim(tmp, "\n");
		free(tmp);
		if (!line)
		{
			ft_fprintf(STDERR_FILENO, "minishell: warning: heredoc at line %d delimited by end-of-file (wanted `%s`)\n", i, heredoc_info->delimiter);
			return (0);
		}
		if (ft_strcmp(line, heredoc_info->delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_tabadd(&heredoc_info->texts, line);
		free(line);
		i++;
	}
	return (1);
}

/**
 * @brief Run the heredoc logic in a fork and handle signals in the child
 *
 * @param t_minishell *minishell
 * @param char *delimiter
 * @param int *output_fd
 * @return int 1 on success, 0 on failure
 */
int	run_heredoc(t_minishell *minishell, char *delimiter, int *output_fd)
{
	pid_t				pid;
	t_heredoc_info		*heredoc_info;
	struct sigaction	sa_child;
	struct sigaction	sa_parent;
	int					status;
	int 				tmp_pipe[2];

	if (pipe(tmp_pipe) == -1)
	{
		return (0);
	}
	status = 0;
	g_exit_code = &minishell->exit_code;
	pid = fork();
	if (pid < 0)
	{
		close(tmp_pipe[0]);
		close(tmp_pipe[1]);
		return (0);
	}
	else if (pid == 0)
	{

		heredoc_info = load_heredoc_info(minishell, tmp_pipe, delimiter);
		if (!heredoc_info)
			exit(1);
		ft_memset(&sa_child, 0, sizeof(sa_child));
		sa_child.sa_flags = 0;
		sa_child.sa_handler = heredoc_signal_handler;
		sigemptyset(&sa_child.sa_mask);
		sigaction(SIGINT, &sa_child, NULL);
		signal(SIGQUIT, SIG_IGN);
		close(tmp_pipe[0]);

		read_heredoc(heredoc_info);

		free(heredoc_info->delimiter);
		close_all_fds(minishell->opened_fds);
		free_minishell(heredoc_info->minishell);

		if (!write_heredoc(heredoc_info))
		{
			close(tmp_pipe[1]);
			ft_tabfree(heredoc_info->texts);
			free(heredoc_info);
			exit(1);
		}

		close(tmp_pipe[1]);
		ft_tabfree(heredoc_info->texts);
		free(heredoc_info);
		exit(0);
	}
	else
	{
		sigemptyset(&sa_parent.sa_mask);
		sa_parent.sa_flags = 0;
		sa_parent.sa_handler = parent_signal_handler;
		sigaction(SIGINT, &sa_parent, NULL);
		signal(SIGQUIT, SIG_IGN);
		close(tmp_pipe[1]);
		waitpid(pid, &status, 0);
		*output_fd = tmp_pipe[0];
		if (minishell->exit_code == 130)
		{
			close(*output_fd);
			return (0);
		}
		else
			minishell->exit_code  = 0;
		return (1);
	}
}
