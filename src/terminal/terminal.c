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

#include "minishell.h"

void	erase_term(size_t len)
{
	size_t		i;

	i = 0;
	while (i < len)
	{
		ft_putstr_fd("\033[1D", 1);
		i++;
	}
	ft_putchar_fd(' ', 1);
	ft_putstr_fd("\033[1D", 1);
}

void	terminal_print(char *str, int nl)
{
	if (nl)
		ft_printf("\033[1000D", str);
	while (nl--)
		ft_putchar_fd('\n', 1);
	ft_printf("%s", str);
}

void	reset_input(char **input)
{
	if (*input)
		free(*input);
	*input = malloc(sizeof(char));
	(*input)[0] = '\0';
}

void	get_cursor_position(int *rows, int *cols)
{
	char			buf[32];
	unsigned int	i;
	int				ret;

	i = 0;
	// Envoyer la séquence d'échappement pour demander la position du curseur
	write(STDOUT_FILENO, "\033[6n", 4);
	// Lire la réponse du terminal
	while (i < sizeof(buf) - 1)
	{
		ret = read(STDIN_FILENO, buf + i, 1);
		if (ret != 1 || buf[i] == 'R')
			break ;
		i++;
	}
	buf[i] = '\0';
	// Analyser la réponse du terminal
	if (buf[0] == '\033' && buf[1] == '[')
		sscanf(buf + 2, "%d;%d", rows, cols);
}

int	interpret_escape_sequence(char *input, int cols)
{
	char	seq[2];

	if (read(STDIN_FILENO, &seq[0], 1) == -1)
		return (-1);
	if (read(STDIN_FILENO, &seq[1], 1) == -1)
		return (-1);
	if (seq[0] == '[')
	{
		if (seq[1] == 'A')
			ft_putstr_fd("\033A", 1);
		else if (seq[1] == 'B')
			ft_putstr_fd("\033B", 1);
		else if (seq[1] == 'C' && cols < ft_strlen(input) + ft_strlen(TERMINAL_PROMPT) + 1)
			ft_putstr_fd("\033[1C", 1);
		else if (seq[1] == 'D' && cols > ft_strlen(TERMINAL_PROMPT) + 1)
			ft_putstr_fd("\033[1D", 1);
		return (1);
	}
	return (0);
}

int	process_action(t_minishell *minishell, char c, char **input, int cols)
{
	if (c == 3 || c == 4)
	{
		return (1);
	}
	else if (c == 127)
	{
		if (ft_strlen(*input) > 0)
		{
			ft_trunc(input, 1);
			erase_term(1);
		}
	}
	else if (c == '\r' || c == '\n')
	{
		if (exec_command(minishell, *input))
			return (1);
		reset_input(input);
		terminal_print(TERMINAL_PROMPT, 1);
	}
	else if (c == '\033') //[ESC]
	{
		if (interpret_escape_sequence(*input, cols))
			return (0);
	}
	else
	{
		//TODO: Recup les col et raw avec la fonction pour savoir ou faire le split
		//TODO: join le nouveau char à la fin de input
		//TODO: split en 2 dans le cas d'un insert
		*input = ft_charjoin(*input, c);
		ft_putchar_fd(c, 1);
	}
	return (0);
}

int	use_termios(t_minishell *minishell)
{
	char	*input;
	char	c;
	int		rows;
	int		cols;
	int		res;

	input = NULL;
	reset_input(&input);
	terminal_print(TERMINAL_PROMPT, 1);
	while (1)
	{
		get_cursor_position(&rows, &cols);
		if (read(STDIN_FILENO, &c, 1) == -1)
		{
			perror("read");
			return (1);
		}
		if (process_action(minishell, c, &input, cols))
			break ;
	}
	terminal_print("Goodbye !", 1);
	terminal_print("", 1);
	free(input);
	return (0);
}
