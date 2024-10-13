/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
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
 *	Save cursor position, insert new character, restore cursor position
 *	[u[C
 * @return void
 */

/**
* @brief
*	get position in window, save it, search equivalence in input
*	erase from position,
* 	join input with new input, print from equivalence to end of input
*
*/


/**
* @brief Clear term from cursor to end
*	Function i did before realised that \033[J is enough
*/
void clear_term(t_minishell *minishell)
{
  unsigned int rows;

    rows = minishell->term->rows;
	while (rows > minishell->term->begin_rows)
		{
			ft_putstr_fd("\033[K\033[1A", 1);
			rows--;
		}
    ft_putstr_fd("\033[K", 1);
}

/**
* 	@brief add new in minishell input
*/

void put_in_string(t_minishell *minishell, char *new)
{
    unsigned int current_cols;
    unsigned int current_rows;
    unsigned int tablen;
	unsigned int prompt_len;

    current_cols = minishell->term->cols;
    current_rows = minishell->term->rows;
	prompt_len = get_prompt_len(minishell);
    ft_putstr_fd("\033[s", 1);
    if (current_rows - minishell->term->begin_rows == 0)
	{
		minishell->input = ft_tabinsert(minishell->input, new,
			minishell->term->cols - prompt_len - 1);
        clear_term(minishell);
		ft_putstr_fd(ft_utf8_tab_to_str(minishell->input) + (minishell->term->cols - prompt_len - 1), STDOUT_FILENO);
	}
    else
    {
        minishell->input = ft_tabinsert(minishell->input, new,
            ((current_rows - minishell->term->begin_rows) * minishell->term->ws_cols) - prompt_len + current_cols - 2);
        minishell->term->rows -= current_rows - minishell->term->begin_rows;
        clear_term(minishell);
        minishell->term->rows += current_rows - minishell->term->begin_rows;
        ft_putstr_fd(ft_utf8_tab_to_str(minishell->input) + ((current_rows - minishell->term->begin_rows) * minishell->term->ws_cols - 1) - prompt_len + current_cols -1, STDOUT_FILENO);
    }
    tablen = ft_tablen((const char **)minishell->input);
    while (tablen + prompt_len > minishell->term->ws_cols * minishell->term->rows - 1)
      {
		ft_putstr_fd("\033[A\r", 1);
        tablen = tablen - minishell->term->ws_cols;
      }
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
	ft_tabdel(minishell->input,
		(unsigned int)cols - get_prompt_len(minishell) - 2);
	reset_stdin(minishell);
	res = ft_utf8_tab_to_str(minishell->input);
	terminal_print(res, 0, STDOUT_FILENO);
	free(res);
	ft_putstr_fd("\033[u", 1);
	if (cols > get_prompt_len(minishell) + 1)
		ft_putstr_fd("\033[1D", 1);
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
	unsigned int    prompt_len;

	input_len = ft_tablen((const char **)minishell->input);
	prompt_len = get_prompt_len(minishell);
	if ((minishell->term->rows - minishell->term->begin_rows == 0 && minishell->term->cols - prompt_len < input_len + 1 )||
    	(minishell->term->rows - minishell->term->begin_rows != 0 &&  ((minishell->term->rows - minishell->term->begin_rows) * minishell->term->ws_cols - 1) - prompt_len + minishell->term->cols < input_len))
	{
		put_in_string(minishell, new);
	}
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
	minishell->term->cols++;
	if (minishell->term->cols >= minishell->term->ws_cols + 1)
	{
      	ft_putstr_fd("\n", 1);
		minishell->term->cols = 1;
		minishell->term->rows++;
	}
}

/**
 * @brief Erase a char in a string
 *
 * TODO: replace this function by a move_cursor function
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
}

