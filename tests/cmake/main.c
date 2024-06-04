#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>

void configureTerminal(struct termios *original) {
    struct termios new_settings;

    tcgetattr(STDIN_FILENO, original);

    new_settings = *original;
    new_settings.c_lflag &= ~(ECHO | ICANON | ISIG);
    new_settings.c_cc[VERASE] = 127;
    new_settings.c_iflag &= ~(ICRNL | INLCR | IGNCR);
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

void resetTerminal(struct termios *original) {
    tcsetattr(STDIN_FILENO, TCSANOW, original);
}

int ft_atoi(const char *str)
{
    int result = 0;
    while (*str && isdigit(*str))
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

void get_cursor_position(int *rows, size_t *cols)
{
    char buf[32];
    unsigned int i = 0;
    int ret;

    // Envoyer la séquence d'échappement pour demander la position du curseur
    write(STDOUT_FILENO, "\033[6n", 4);

    // Lire la réponse du terminal
    while (i < sizeof(buf) - 1)
    {
        ret = read(STDIN_FILENO, buf + i, 1);
        if (ret != 1 || buf[i] == 'R')
            break;
        i++;
    }
    buf[i] = '\0';

    // Vérifier et analyser la réponse
    if (buf[0] == '\033' && buf[1] == '[')
    {
        i = 2;
        *rows = ft_atoi(buf + i);
        while (buf[i] && isdigit(buf[i]))
            i++;
        if (buf[i] == ';')
        {
            *cols = ft_atoi(buf + i + 1);
        }
    }
}

int main() {
    struct  termios original;
    char    c;

    configureTerminal(&original);

    printf("Press any key to continue...\n");
    read(STDIN_FILENO, &c, 1);
    printf("You pressed: %c\n", c);

    resetTerminal(&original);
    return 0;
}
