//
// Created by Julie on 02/06/2024.
//

#include "../include/termios.h"

void disable_raw_mode(t_termios *termios) {
    // Restaurer les attributs du terminal original
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, termios->original_termios) == -1) {
        perror("tcsetattr");
		return ;
    }
}

void enable_raw_mode(t_termios *termios) {
    struct termios raw;

    // Obtenir les attributs du terminal actuel
    if (tcgetattr(STDIN_FILENO, termios->original_termios) == -1) {
        perror("tcgetattr");
        return;
    }

    // Copier les attributs pour les modifier
    raw = *termios->original_termios;

    // Modifier les attributs pour le mode brut
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON | ICRNL);
    raw.c_cflag |= (CS8);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    // Appliquer les nouveaux attributs du terminal
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        return;
    }
}

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*cps1;
	unsigned char	*cps2;

	cps1 = (unsigned char *)s1;
	cps2 = (unsigned char *)s2;
	while (*cps1 || *cps2)
	{
		if (*cps1 != *cps2)
			return (*cps1 - *cps2);
		cps1++;
		cps2++;
	}
	return (0);
}

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_charjoin(char *str, char c)
{
	char	*dst;
	int		i;

	dst = (char *)malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!dst)
		return (NULL);
	i = 0;
	while (i < ft_strlen(str))
	{
		dst[i] = str[i];
		i++;
	}
	dst[i] = c;
	i++;
	dst[i] = '\0';
	if (str)
		free(str);
	return (dst);
}

int	ft_putchar_fd(char c, int fd)
{
	return(write(fd, &c, 1));
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i])
		write(fd, &str[i++], 1);
}

void	ft_trunc(char *str, size_t era)
{
	size_t	len;
	int		i;

	len = ft_strlen(str);
	if (len < era)
		return ;
	i = len - era - 1;
	if (i < 0)
		i = 0;
	while(str[i])
	{
		str[i] = '\0';
		i++;
	}
}

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

int	wait_input(char **input, t_termios *termios)
{
    char c;

	*input = malloc(sizeof(char) * 1);
	if (!*input)
		return (0);
	*input[0] = '\0';
	while (1) {
        if (read(STDIN_FILENO, &c, 1) == -1) {
            perror("read");
            disable_raw_mode(termios);
        }
		*input = ft_charjoin(*input, c);
		ft_putchar_fd(c, 1);
		if (c == 4 || c == 3 || c == 28 || ft_strcmp(*input, "exit") == 13)
		{
			ft_putstr_fd("\033[E", 1);
			erase_term(ft_strlen(*input));
			return (0);
		}
		//UP: 65, DOWN: 66, RIGHT: 67, LEFT: 68
		else if (c == 13)
			break ;
		else if(c == 66 || c == 65)
		{
			//history management
			break ;
		} else if (c == 67 || c == 68)
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

int main() {
    struct termios original_termios;
    t_termios termios;
	char *cmd;

    termios.original_termios = &original_termios;
    enable_raw_mode(&termios);
	while (1)
	{
		if (!wait_input(&cmd, &termios))
		{
			disable_raw_mode(&termios);
			free(cmd);
			break ;
		}
		erase_term(ft_strlen(cmd));
		free(cmd);
	}
    disable_raw_mode(&termios);
    return 0;
}
