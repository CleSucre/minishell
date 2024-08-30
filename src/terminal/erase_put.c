/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   erase_put.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 05:39:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/19 03:19:59 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Add a char in string at "cols" (n) position
 * 			and put back the cursor at the right place
 *
 * @return void
 */
void	put_in_string(t_minishell *minishell, char *new)
{
	char 			*str;

    minishell->input = ft_tabinsert(minishell->input, new, minishell->term->cols - get_prompt_len(minishell) - 1);
	ft_putstr_fd("\033[s", 1);
	reset_stdin(minishell);
	str = ft_utf8_tab_to_str(minishell->input);
	ft_fprintf(STDOUT_FILENO, str );
	free(str);
	ft_putstr_fd("\033[u\033[1C", 1);
}

/**
 * @brief Delete a char in string at "cols" (n) position
 * 			and put back the cursor at the right place
 *
 * @param char *input	String to delete char
 * @param size_t cols	Position to delete char
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
	ft_tabdel(minishell->input, (unsigned int)cols - get_prompt_len(minishell) - 2);
	reset_stdin(minishell);
	res = ft_utf8_tab_to_str(minishell->input);
	terminal_print(res, 0, STDOUT_FILENO);
	free(res);
	ft_putstr_fd("\033[u", 1);
	if (cols > get_prompt_len(minishell) + 1)
		ft_putstr_fd("\033[1D", 1);
}

/**
 * @brief Erase a char in a string
 *
 * @param size_t len
 * @return void
 */
void	erase_term(size_t len)
{
	size_t		i;

	i = 0;
	while (i < len)
	{
		ft_printf(CURSOR_LEFT, 1);
		i++;
	}
	ft_putchar_fd(' ', 1);
	ft_printf(CURSOR_LEFT, 1);
}
