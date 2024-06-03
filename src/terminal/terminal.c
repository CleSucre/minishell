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

static void	interpret_escape_sequence(char *input)
{
	char	seq[3];

	(void)input;
	if (read(STDIN_FILENO, &seq[0], 1) == -1)
		return ;
	if (read(STDIN_FILENO, &seq[1], 1) == -1)
		return ;
	if (seq[0] == '[')
	{
		if (seq[1] >= 'A' && seq[1] <= 'D')
		{
			if (seq[1] == 'A')
				terminal_print("Flèche Haut", 1);
			else if (seq[1] == 'B')
				terminal_print("Flèche Bas", 1);
			else if (seq[1] == 'C')
				terminal_print("Flèche Droite", 1);
			else if (seq[1] == 'D')
				terminal_print("Flèche Gauche", 1);
		}
	}
}

int	process_action(t_minishell *minishell, char c, char **input)
{
	if (c == 3 || c == 4)
	{
		return (1);
	}
	else if (c == 127)
	{
		if (ft_strlen(*input) > 0)
		{
			*input = ft_substr(*input, 0, ft_strlen(*input) - 1);
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
	else if (c == '\x1b')
		interpret_escape_sequence(*input);
	else
	{
		*input = ft_charjoin(*input, c);
		ft_putchar_fd(c, 1);
	}
	return (0);
}

int	use_termios(t_minishell *minishell)
{
	char	*input;
	char	c;

	input = NULL;
	reset_input(&input);
	terminal_print(TERMINAL_PROMPT, 1);
	while (1)
	{
		if (read(STDIN_FILENO, &c, 1) == -1)
		{
			perror("read");
			return (1);
		}
		if (process_action(minishell, c, &input))
			break ;
	}
	terminal_print("Goodbye !", 1);
	terminal_print("", 1);
	free(input);
	return (0);
}
