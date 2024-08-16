/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_arrow.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 04:02:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 03:58:37 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Process action of cursor up
 *
 * @param t_minishell *minishell
 * @param char **input
 */
void	arrow_up_action(t_minishell *minishell,
		char ***input, t_history *new_history)
{
	char	*cmd;

	if (minishell->history_pos == 0)
	{
		ft_tabfree(minishell->cache->input);
		minishell->cache->input = ft_tabdup((const char **)*input);
	}
	cmd = ft_utf8_tab_to_str(minishell->cache->input);
	new_history = history_find_up(minishell, cmd);
	if (new_history && new_history->cmd)
	{
		ft_tabfree(*input);
		*input = ft_utf8_split_chars(new_history->cmd);
		ft_putstr_fd("\033[1000D", STDOUT_FILENO);
		terminal_print("\033[2K", 0, STDOUT_FILENO);
		print_terminal_prompt(minishell, 0);
		terminal_print(new_history->cmd, 0, STDOUT_FILENO);
	}
	free(cmd);
	get_cursor_position(minishell->term);
}

/**
 * @brief Process action of cursor down
 *
 * @param t_minishell *minishell
 * @param char **input
 */
void	arrow_down_action(t_minishell *minishell,
			char ***input, t_history *new_history)
{
	char	*cmd;

	cmd = NULL;
	if (minishell->history_pos == 0)
	{
		ft_tabfree(minishell->cache->input);
		minishell->cache->input = ft_tabdup((const char **)*input);
	}
	cmd = ft_utf8_tab_to_str(minishell->cache->input);
	new_history = history_find_down(minishell, cmd);
	if (new_history && new_history->cmd)
	{
		free(cmd);
		cmd = ft_strdup(new_history->cmd);
	}
	ft_tabfree(*input);
	*input = ft_utf8_split_chars(cmd);
	ft_putstr_fd("\033[1000D", 1);
	terminal_print("\033[2K", 0, STDOUT_FILENO);
	print_terminal_prompt(minishell, 0);
	terminal_print(cmd, 0, STDOUT_FILENO);
	free(cmd);
	get_cursor_position(minishell->term);
}

void	arrow_left_action(t_minishell *minishell)
{
	minishell->term->cols--;
	ft_putstr_fd("\033[1D", 1);
}

void	arrow_right_action(t_minishell *minishell)
{
	minishell->term->cols++;
	ft_putstr_fd("\033[1C", 1);
}
