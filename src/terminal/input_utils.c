/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 05:39:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/13 10:26:07 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Delete a char in string at "cols" (n) position
 * 			and put back the cursor at the right place
 *
 * @param t_minishell *minishell
 * @return void
 */
void	erase_in_string(t_minishell *minishell)
{
	char			*res;
	unsigned int	cols;

	cols = minishell->term->cols;
	if (cols <= get_prompt_len(minishell))
		return ;
	ft_putstr_fd("\033[s", 1);
	ft_tabdel(minishell->input, (unsigned int)cols - get_prompt_len(minishell)
		- 2);
	reset_stdin(minishell);
	res = ft_utf8_tab_to_str(minishell->input);
	terminal_print(res, 0, STDOUT_FILENO);
	free(res);
	ft_putstr_fd("\033[u", 1);
	if (cols > get_prompt_len(minishell) + 1)
		ft_putstr_fd("\033[1D", 1);
}

/**
 * @brief Add a new line if the cursor is at the end of the window
 * 			if cursor at end of cols, go one line under
 */
static void	ad_nl_last_rows(t_minishell *minishell, unsigned int input_len,
		unsigned int prompt_len, unsigned int new_len)
{
	minishell->term->cols += new_len;
	if (minishell->term->cols >= minishell->term->ws_cols + 1)
	{
		ft_putstr_fd("\033[E", 1);
		minishell->term->cols %= minishell->term->ws_cols;
		move_cursor_forward(minishell->term->cols - 1);
		minishell->term->rows++;
	}
	if ((input_len + prompt_len)
		% minishell->term->ws_cols == minishell->term->ws_cols - 2
		&& minishell->term->rows >= minishell->term->ws_rows)
	{
		ft_putstr_fd("\033[s", 1);
		ft_fprintf(STDOUT_FILENO, "\033[%dB", minishell->term->ws_rows - 1);
		ft_putstr_fd("\n\033[u\033[A", 1);
	}
}

/**
 * @brief Edit the input string with the new char
 * 		according to the cursor position
 *
 * @param t_minishell *minishell
 * @param char *new
 */
void	edit_input(t_minishell *minishell, char *new)
{
	unsigned int	input_len;
	unsigned int	prompt_len;

	input_len = ft_tablen((const char **)minishell->input);
	prompt_len = get_prompt_len(minishell);
	if ((minishell->term->rows - minishell->term->begin_rows == 0
			&& minishell->term->cols - prompt_len < input_len + 1)
		|| (minishell->term->rows - minishell->term->begin_rows != 0
			&& ((minishell->term->rows - minishell->term->begin_rows)
				* minishell->term->ws_cols - 1) - prompt_len
			+ minishell->term->cols < input_len))
		put_in_string(minishell, new);
	else
	{
		ft_putstr_fd(new, STDOUT_FILENO);
		minishell->input = ft_tabjoin(minishell->input,
				ft_utf8_split_chars(new));
		if (minishell->tab_dict)
			free_branch(minishell->tab_dict);
		minishell->tab_dict = NULL;
	}
	minishell->completion->tab_count = 0;
	minishell->completion->check_len = 0;
	minishell->completion->print_line = 1;
	ad_nl_last_rows(minishell, input_len, prompt_len, ft_strlen(new));
}

/**
 * @brief Erase a char in a string
 *
 * @param size_t len
 */
void	erase_term(size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		ft_printf(CURSOR_LEFT, 1);
		i++;
	}
	ft_putchar_fd(' ', 1);
}
