/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
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

// TODO: Je te laisse remplacer rows et cols par minishell->terms->rows etc. comme tu auras decider de les nommer
void get_terminal_size(t_term *term)
{
    struct winsize size;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    term->rows = size.ws_row;
    term->cols = size.ws_col;
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
