#include "minishell.h"

/**
 * @brief Gestionnaire de signal pour SIGINT (Ctrl+C) et SIGQUIT (Ctrl+\)
 *
 * @param sig Signal capturé
 */
void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		signal(SIGINT, SIG_DFL);
	}
	else if (sig == SIGQUIT)
	{
		ft_putstr_fd("WTF\n", STDOUT_FILENO);
		signal(SIGQUIT, SIG_DFL);
	}
}

/**
 * @brief Configure les signaux pour la gestion dans les processus enfants
 *
 * @param struct sigaction *sa Structure de signal pour la configuration
 */
void	setup_signals(struct sigaction *sa)
{
	sa->sa_handler = handle_signal;
	sa->sa_flags = 0;
	sigemptyset(&sa->sa_mask);

	if (sigaction(SIGINT, sa, NULL) == -1 || sigaction(SIGQUIT, sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Gérer le processus enfant après fork (commande externe)
 *
 * @param t_cmd *cmd Structure de commande
 * @param t_minishell *minishell Contexte de minishell
 * @param struct sigaction *sa Structure de signal pour la gestion des signaux
 */
static void	handle_child_process(t_cmd *cmd, t_minishell *minishell)
{
	int	err;

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

	err = execute_path(cmd);
	destroy_cmd(cmd);
	free_minishell(minishell);
	exit(err);
}

/**
 * @brief Gérer le processus parent après fork
 *
 * @param t_cmd *cmd Structure de commande
 * @param t_minishell *minishell Contexte de minishell
 */
static void	handle_builtins_child_process(t_cmd *cmd, t_minishell *minishell)
{
	int	err;

	err = execute_builtin_command(minishell, cmd);
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
	destroy_cmd(cmd);
	free_minishell(minishell);
	exit(err);
}

/**
 * @brief Exécute une commande dans un processus enfant
 *        et gère la redirection des entrées/sorties.
 *
 * @param t_minishell *minishell Contexte de minishell
 * @param t_cmd *cmd Structure de commande
 * @return int Code de sortie
 */
int	execute_external(t_minishell *minishell, t_cmd *cmd)
{
	struct sigaction	sa;
	pid_t				pid;

	setup_signals(&sa);

	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("Error: fork failed\n", STDERR_FILENO);
		destroy_cmd(cmd);
		return (1);
	}
	else if (pid == 0)
	{
		if (is_builtin_command(cmd))
			handle_builtins_child_process(cmd, minishell);
		else
			handle_child_process(cmd, minishell);
	}

	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);

	return (pid);
}
