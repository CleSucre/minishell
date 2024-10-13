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
 * @param t_history *new_history
 */
void	arrow_up_action(t_minishell *minishell, t_history *new_history)
{
	char	*cmd;

	if (minishell->history_pos == 0)
	{
		ft_tabfree(minishell->cache->input);
		minishell->cache->input = ft_tabdup((const char **)minishell->input);
	}
	cmd = ft_utf8_tab_to_str(minishell->cache->input);
	new_history = history_find_up(minishell, cmd);
	if (new_history && new_history->cmd)
	{
		ft_tabfree(minishell->input);
		minishell->input = ft_utf8_split_chars(new_history->cmd);
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
 * @param t_history *new_history
 */
void	arrow_down_action(t_minishell *minishell, t_history *new_history)
{
	char	*cmd;

	cmd = NULL;
	if (minishell->history_pos == 0)
	{
		ft_tabfree(minishell->cache->input);
		minishell->cache->input = ft_tabdup((const char **)minishell->input);
	}
	cmd = ft_utf8_tab_to_str(minishell->cache->input);
	new_history = history_find_down(minishell, cmd);
	if (new_history && new_history->cmd)
	{
		free(cmd);
		cmd = ft_strdup(new_history->cmd);
	}
	ft_tabfree(minishell->input);
	minishell->input = ft_utf8_split_chars(cmd);
	ft_putstr_fd("\033[1000D", 1);
	terminal_print("\033[2K", 0, STDOUT_FILENO);
	print_terminal_prompt(minishell, 0);
	terminal_print(cmd, 0, STDOUT_FILENO);
	free(cmd);
	get_cursor_position(minishell->term);
}

/**
 * @brief Process action of cursor left
 *
 * @param t_minishell *minishell
 */
void	arrow_left_action(t_minishell *minishell)
{
	t_term *term = minishell->term;
	if (term->rows - term->begin_rows == 0 && term->cols <= get_prompt_len(minishell) + 1)
			return ;
	if (term->rows - term->begin_rows != 0 && term->cols <= 1)
	{
		ft_putstr_fd("\033[F", 1);
		ft_printf("\033[%dG", term->ws_cols);
		term->cols = term->ws_cols;
		term->rows--;
		ft_putstr_fd("\033[1;33m", 1);
		return ;
	}
	ft_putstr_fd("\033[D", 1);
	term->cols--;
}

/**
 * @brief Process action of cursor right
 *
 * @param t_minishell *minishell
 */
void	arrow_right_action(t_minishell *minishell)
{
	t_term 			*term;
	unsigned int	input_len;

	term  = minishell->term;
	input_len = (unsigned int)ft_tablen((const char **)minishell->input);
	if (term->rows - term->begin_rows == 0 && term->cols - get_prompt_len(minishell) >= input_len + 1)
		return ;
	else if (term->rows - term->begin_rows != 0 &&  ((term->rows - term->begin_rows) * term->ws_cols) - get_prompt_len(minishell) + term->cols >= input_len + 2)
		return ;
	if (term->cols >= term->ws_cols)
	{
		ft_putstr_fd("\033[E", 1);
		term->cols = 1;
		term->rows++;
		ft_putstr_fd("\033[1;32m", 1);
		return ;
	}
	ft_putstr_fd("\033[C", 1);
	term->cols++;
}
