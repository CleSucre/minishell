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

int	wait_input(char **input, t_minishell *minishell)
{
    char c;

	*input = malloc(sizeof(char) * 1);
	if (!*input)
		return (0);
	*input[0] = '\0';
	while (1) {
        if (read(STDIN_FILENO, &c, 1) == -1) {
            perror("read");
            disable_raw_mode(minishell->termios);
        }
		*input = ft_charjoin(*input, c);
		ft_putchar_fd(c, 1);
		if (c == 4 || c == 3 || c == 28 || ft_strcmp(*input, "exit") == 13)
		{
			ft_putstr_fd("\033[E", 1);
			erase_term(ft_strlen(*input));
			return (0);
		}
		else if (c == 13)
			break ;
		else if(c == 66 || c == 65)
		{
			//history management
			break ;
		}
		else if (c == 67 || c == 68)
		{
			//cursor management
			break ;
		}
		else if (c == 127 && ft_strlen(*input) > 0)
		{
			erase_term(1);
			ft_trunc(*input, 1);
		}
    }
	return (1);
}