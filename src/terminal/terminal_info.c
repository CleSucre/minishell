/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_info.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 05:39:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/06/03 05:39:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

/**
 * @brief Get the terminal size
 * @param rows
 * @param cols
 */
void get_terminal_size(t_term *term)
{
    struct winsize size;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    term->size_rows = size.ws_row;
    term->size_cols = size.ws_col;
}


/**
 * @brief Get cursor position in our terminal
 * 			Ask for it
 * 			Read terms answer
 * 			Analyse it
 * @param rows position y
 * @param cols position x
 */
void	get_cursor_position(t_term *term)
{
	char			buf[32];
	unsigned int	i;
	int				ret;

	i = 0;
	write(STDOUT_FILENO, "\033[6n", 4);
	while (i < sizeof(buf) - 1)
	{
		ret = read(STDIN_FILENO, buf + i, 1);
		if (ret != 1 || buf[i] == 'R')
			break ;
		i++;
	}
	buf[i] = '\0';
	if (buf[0] == '\033' && buf[1] == '[')
	{
		i = 2;
		term->rows = ft_atoi(buf + i);
		while (buf[i] && ft_isdigit(buf[i]))
			i++;
		term->cols = ft_atoi(buf + i + 1);
	}
}

//TODO: Create functions to replaces theses above



/**
 * @brief Init terminfo and use it to fill minishell->term infos
 */


//void	term_info(t_minishell *minishell)
//{
//	char	*term_type;
//	int		lib_access;
//
//	term_type = getenv("TERM");
//	minishell->term->iterm_buffer = ft_calloc(sizeof(char*) * 2048, 1);
//	minishell->term->strterm_buffer = ft_calloc(sizeof(char*) * 2048, 1);
//	if(!minishell->term->iterm_buffer || !minishell->term->strterm_buffer)
//	{
//		ft_fprintf(2, "Memory allocation failed");
//		exit(EXIT_FAILURE);
//	}
//	if(!term_type)
//	{
//		ft_fprintf(2, "Can't access TERM info because of env");
//		exit(EXIT_FAILURE);
//	}
//	lib_access = my_tgetent(minishell->term, term_type);
//	if (lib_access < 1)
//	{
//		ft_fprintf(2, "Can't access terminfo database or term_type not found in database");
//		exit(EXIT_FAILURE);
//	}
//}

/**
 * @brief Move cursor
 *		togo --> generate control sequence with wishes coordinates
 *		tputs --> send control sequence to  terms
 */

//
//void	move_cursor(t_term *term, int x, int y)
//{
//	char	*move_sequence;
//	char	*move_action;
//	char	*buffer;
//
//	buffer = ft_calloc(sizeof(char*) * 2048, 1);
//	move_sequence = tgetstr("cm", &term->strterm_buffer);
//	if(!move_sequence)
//	{
//
//		ft_fprintf(2, "Impossible to get cursor move's sequence");
//		exit(EXIT_FAILURE);
//	}
//// 	get_cursor_position(term);
// 	move_action = tgoto(move_sequence, x, y);
// 	tputs(move_action, 1, NULL);
//	 free(buffer);
//}


/**
* @brief Clear screen and reprint printf if necessary
 * cline is for current_line
 * ce ou cd
*/

//TODO: Check : il me semble que des choses ici sont utiles

//void	refresh_cline(t_term *term)
//{
//	char	*clear_lsequence;
//	char	*buffer;
//
//	buffer = ft_calloc(sizeof(char*) * 2048, 1);
//	move_cursor(term, 1, (int)term->rows);
//	clear_lsequence = tgetstr("ce", &buffer);
//	if(!clear_lsequence)
// 	{
// 		ft_fprintf(2, "Impossible to catch clear_lsequence");
// 		exit(EXIT_FAILURE);
// 	}
//	tputs(clear_lsequence, 1, NULL);
//	ft_putstr_fd(TERMINAL_PROMPT, 1);
//	free(buffer);
//}

/**
* @brief Delete 1 char
*/
//int my_putc(int c)
//{
//	(void)c;
//    return(0);
//}
//
//void	del(t_minishell *minishell, char **input)
//{
//	char	*del_sequence;
//    char    *buffer;
//
//	buffer = NULL;
//    buffer = ft_calloc(sizeof(char*) * 2048, 1);
//	if (ft_strlen(*input) > 0 && minishell->term->cols != ft_strlen(TERMINAL_PROMPT) + ft_strlen(*input) + 1)
//	{
//		ft_putstr_fd("\033[s", 1);
//		*input = erase_in_string(*input, minishell->term->cols);
//		ft_putstr_fd("\033[u", 1);
//		if (minishell->term->cols > ft_strlen(TERMINAL_PROMPT) + 1)
//			ft_putstr_fd("\033[1D", 1);
//	}
//	else if (ft_strlen(*input) > 0)
//    {
//        ft_trunc(input, 1);
//        del_sequence = tgetstr("dc", &buffer);
//        if (del_sequence == NULL)
//        {
//            ft_fprintf(2, "Impossible to catch delet_sequence");
//            exit(EXIT_FAILURE);
//        }
//        tputs(del_sequence, 1, my_putc);
//		free(buffer);
//
//    }
//}


/**
* @brief Move cursor to prompt position
*/
//void go_prompt(t_minishell *minishell)
//{
//    move_cursor(minishell->term, (int)ft_strlen(TERMINAL_PROMPT), (int)minishell->term->rows);
//}
