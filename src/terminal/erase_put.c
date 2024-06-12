/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   erase_put.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 05:39:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/06/03 05:39:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Clear the lines and put back prompt after moving cursor
 *
 * @param input
 * @param cols
 */

static void	reset_stdin(t_minishell *minishell, const char *input)
{
	(void)input;
	ft_putstr_fd("\033[2K", 1);
	move_cursor_back(minishell->term->cols);
	terminal_print(minishell->cache->prompt, 0);
}

/**
 * @brief Add a char in string at "cols" (n) position
 * 			and put back the cursor at the right place
 *
 * @param char *input	string to add char
 * @param char c		char to add
 * @param cols			Position to add char
 * @return
 */

char	*put_in_string(t_minishell *minishell, char *input, char c)
{
	char	*res;
	size_t	i;
	int		cols;

	cols = minishell->term->cols;
	res = ft_calloc(ft_strlen(input) + 1, sizeof(char *));
	i = 0;
	ft_putstr_fd("\033[s", 1);
	while (input[i] && i < cols - minishell->cache->prompt_len - 1)
	{
		res[i] = input[i];
		i++;
	}
	res[i++] = c;
	while (input[i - 1])
	{
		res[i] = input[i - 1];
		i++;
	}
	free(input);
	reset_stdin(minishell, res);
	terminal_print(res, 0);
	ft_putstr_fd("\033[u\033[1C", 1);
	return (res);
}

/**
 * @brief Delete a char in string at "cols" (n) position
 * 			and put back the cursor at the right place
 *
 * @param char *input	String to delete char
 * @param size_t cols	Position to delete char
 * @return
 */
char	*erase_in_string(t_minishell *minishell, char *input)
{
	char			*res;
	size_t			i;
	unsigned int	cols;

	cols = minishell->term->cols;
	if (cols <= minishell->cache->prompt_len)
		return (input);
	res = ft_calloc(ft_strlen(input), sizeof(char *));
	i = 0;
	ft_putstr_fd("\033[s", 1);
	while (input[i] && i < cols - minishell->cache->prompt_len - 2)
	{
		res[i] = input[i];
		i++;
	}
	i++;
	while (input[i])
	{
		res[i - 1] = input[i];
		i++;
	}
	reset_stdin(minishell, input);
	free(input);
	terminal_print(res, 0);
	ft_putstr_fd("\033[u", 1);
	if (cols > minishell->cache->prompt_len + 1)
		ft_putstr_fd("\033[1D", 1);
	return (res);
}