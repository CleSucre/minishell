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
*	OLD CODE :
*		ft_printf("\033[s\033[4h%s\033[4l\033[0m", new);
*		minishell->input = ft_tabinsert(minishell->input, new,
*			minishell->term->cols - get_prompt_len(minishell) - 1);
*/


/**
* @brief Clear term from cursor to end
*	Function i did before realised that \033[J is enough
*/
void clear_term(t_minishell *minishell)
{
	while (minishell->term->rows > minishell->term->input_starting_row)
		{
			ft_putstr_fd("\033[K\033[1A", 1);
			minishell->term->rows--;
		}
	ft_putstr_fd("\033[K\033[1A", 1);
}

/**
* @brief Basic function which print input from start to end
*	depend on terminal size
*	@param t_term *term
*	@param char *input
*	@param int start
*	@param int end
*	@return void
*/
void print_str_from_s_to_e_term(t_minishell *minishell, char *input, int start, int end)
{
	int i;
	int rows;

	i = start;
	rows = minishell->term->rows;
	// classic verif input
	if (start >= end)
		{
			ft_putstr_fd("Error\n", 2);
			ft_fprintf(2, "start : %d\nend : %d\n", start, end);
			return ;
		}
	while (input[i] && i < end)
	{
		ft_putchar_fd(input[i], 1);
		i++;
		// check si fin de ligne selon si row est egal au start (on va dire 0) ou pas, si oui on prend en compte le prompt sinon non
		if ((rows == minishell->term->input_starting_row && (i + get_prompt_len(minishell)) % minishell->term->ws_cols == 0)
			|| (rows > minishell->term->input_starting_row && (i % minishell->term->ws_cols == 0)))
			{
//                ft_putstr_fd("\033[E", 1);
                rows++;
            }
	}

}

/**
* 	@brief add new in minishell input
*	Save cursor position and clear term from cursor to end
*	then search position in input according to cursor
*	then print from position to end of input
*	Restore position, check if needed to go to next line or move to right
*/
//void    put_in_string(t_minishell *minishell, char *new)
//{
//    char    *str;
//	unsigned int current_pos;
//	unsigned int cols;
//	unsigned int rows;
//
//	cols = minishell->term->cols;
//	rows = minishell->term->rows;
//
//
//	// insertion dans l input
//    minishell->input = ft_tabinsert(minishell->input, new, minishell->term->cols - get_prompt_len(minishell) - 1);
//	// sauvegarde position + clear term
//    ft_putstr_fd("\033[s\033[J", 1);
//    str = ft_utf8_tab_to_str(minishell->input);
//
//	// Position du curseur : si row == starting alors juste cols - prompt len sinon calcul
//	if (minishell->term->rows == minishell->term->input_starting_row)
//        current_pos = minishell->term->cols - get_prompt_len(minishell) - 1;
//    else
//	current_pos = minishell->term->cols * (minishell->term->rows -
//		minishell->term->input_starting_row) - get_prompt_len(minishell) - 1;
//
//	// print de position jusqua fin de l input
//	print_str_from_s_to_e_term(minishell, str, current_pos, ft_strlen(str));
//
//    free(str);
//
//	// restauration de base du curseur avec U
//    ft_putstr_fd("\033[u", 1);
//	if (minishell->term->cols % minishell->term->ws_cols == 0)
//    {
//        ft_putstr_fd("\033[E", 1);
//        minishell->term->rows++;
//        minishell->term->cols = 1;
//    }
//	else
//			ft_putstr_fd("\033[1C", 1);
//
//
//}

void    put_in_string(t_minishell *minishell, char *new)
{
	char	*str;

	ft_putstr_fd("\033[s\033[J", 1);
	minishell->input = ft_tabinsert(minishell->input, new, minishell->term->cols - get_prompt_len(minishell) - 1);
	reset_stdin(minishell);
	str = ft_utf8_tab_to_str(minishell->input);
	terminal_print(str, 0, STDOUT_FILENO);
	free(str);
	if (minishell->term->rows - minishell->term->input_starting_row > 0)
		ft_printf("\033[%dB\033[G", minishell->term->rows - minishell->term->input_starting_row);
	ft_putstr_fd("\033[u", 1);
		if (minishell->term->cols % minishell->term->ws_cols == 0)
    {
        ft_putstr_fd("\033[E", 1);
        minishell->term->rows++;
        minishell->term->cols = 1;
    }
	else if (minishell->term->cols > get_prompt_len(minishell) + 1)
        ft_putstr_fd("\033[1C", 1);
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
	if (minishell->term->cols
		!= get_prompt_len(minishell)
		+ ft_tablen((const char **)minishell->input) + 1)
		{
			put_in_string(minishell, new);
		}
	else
	{
		minishell->input = ft_tabjoin(minishell->input,
				ft_utf8_split_chars(new));
		ft_putstr_fd(new, STDOUT_FILENO);
		if (minishell->tab_dict)
			free_branch(minishell->tab_dict);
		minishell->tab_dict = NULL;
	}
	minishell->term->cols ++;
	// incrementation de rows si a la fin de ligne apres input
	minishell->term->rows = (ft_tablen((const char **)minishell->input) + get_prompt_len(minishell))
        / minishell->term->ws_cols + minishell->term->input_starting_row;
	minishell->completion->tab_count = 0;
	minishell->completion->check_len = 0;
	minishell->completion->print_line = 1;
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

