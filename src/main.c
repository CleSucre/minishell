/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/22 21:07:51 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the shell is running in
 * 			a pipe (stdin or stdout not a terminal)
 *
 * @return int 1 if stdin or stdout is not a terminal, 0 otherwise.
 */
int	is_running_in_pipeline(void)
{
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (1);
	return (0);
}

int	fill_env(t_minishell *minishell)
{
	char	*tmp;

	minishell->env = ft_calloc(2, sizeof(char *));
	minishell->env[0] = ft_strdup("PATH=/usr/bin:/bin:/usr/sbin:/sbin");
	tmp = ft_strjoin("PWD=", getcwd(NULL, 0));
	minishell->env = ft_tabinsert(minishell->env, tmp, 1);
	free(tmp);
	minishell->env = ft_tabinsert(minishell->env, "OLDPWD=", 2);
	minishell->env = ft_tabinsert(minishell->env, "SHLVL=1", 3);
	minishell->env = ft_tabinsert(minishell->env, "_=", 4);
	minishell->env = ft_tabinsert(minishell->env, "TERM=xterm-256color", 5);
	minishell->env = ft_tabinsert(minishell->env, "HOME=", 6);
	minishell->env = ft_tabinsert(minishell->env, "USER=", 7);
	return (0);
}

/**
 * @brief Main function of the minishell
 *
 * @param int argc
 * @param char **args
 * @param char **env
 * @return int
 */
int	main(int argc, char **args, char **env)
{
	t_minishell		*minishell;
	struct termios	original_termios;
	int				exit_code;

	(void)argc;
	(void)args;
	if (is_running_in_pipeline())
		return (1);
	minishell = alloc_minishell();
	minishell->term->original_termios = &original_termios;
	if (ft_tablen((const char **)env))
		minishell->env = ft_tabdup((const char **)env);
	else
		fill_env(minishell);
	enable_termios(minishell->term);
	use_termios(minishell);
	disable_termios(minishell->term);
	exit_code = minishell->exit_code;
	close_all_fds(minishell->opened_fds);
	free_minishell(minishell);
	return (exit_code);
}
