#include "minishell.h"

void	erase_term(size_t len)
{
	size_t		i;

	i = 0;
	while(i < len)
	{
		ft_putstr_fd("\033[1D", 1);
		i++;
	}
	ft_putchar_fd(' ', 1);
	ft_putstr_fd("\033[1D", 1);
}

void terminal_print(char *str, int nl)
{
	ft_printf("\033[1000D");
	while (nl--)
		ft_putchar_fd('\n', 1);
	ft_printf("%s", str);
}

static void interpret_escape_sequence(void) {
	char	seq[3];

	if (read(STDIN_FILENO, &seq[0], 1) == -1)
		return;
	if (read(STDIN_FILENO, &seq[1], 1) == -1)
		return;
	if (seq[0] == '[') {
		if (seq[1] >= 'A' && seq[1] <= 'D') {
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

int process_action(t_minishell *minishell, char c, char **input)
{
	if (c == 3 || c == 4)
	{
		return (1);
	}
	else if (c == 127)
	{
		ft_trunc(*input, ft_strlen(*input) - 1);
		erase_term(1);
	}
	else if (c == '\r' || c == '\n')
	{
		if (exec_command(minishell, *input))
			return (1);
		terminal_print(TERMINAL_PROMPT, 1);
	}
	else if (c == '\x1b')
		interpret_escape_sequence();
	else
	{
		*input = ft_strjoin(*input, &c);
		ft_putchar_fd(c, 1);
	}
	return (0);
}

int	use_termios(t_minishell *minishell, char **input)
{
    char c;

	*input = malloc(sizeof(char) * 1);
	if (!*input)
		return (1);
	*input[0] = '\0';

	terminal_print(TERMINAL_PROMPT, 1);
	while (1)
	{
		if (read(STDIN_FILENO, &c, 1) == -1)
		{
            perror("read");
			return (1);
        }
		if (process_action(minishell, c, input))
			break ;
    }
	terminal_print("exit", 2);
	return (0);
}