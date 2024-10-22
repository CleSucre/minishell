/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 06:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	str_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}


static int	check__digit(char *str, char **endptr, int base)
{
	int			i;
	int			digit;
	long long	res;

	i = 0;
	res = 0;
	while (ft_isdigit(str[i]))
	{
		digit = str[i] - '0';
		if (res > (LLONG_MAX - digit) / base)
		{
			*endptr = (char *)&str[i];
			return (i);
		}
		res = res * base + digit;
		i++;
	}
	return (0);
}

/**
* @brief Convert a string to a long long integer
* @param str String to convert
* @param endptr Pointer to the character that stops the conversion
* @param base Base of the number
* @return long long Converted number
*/
static long long	ft_strtoll(const char *str, char **endptr, int base)
{
    long long	res;
    int			sign;
    int			i;

    i = 0;
    res = 0;
    sign = 1;
    while (ft_isspace(str[i]))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
	if (check__digit((char *)&str[i], endptr, base))
		{
			*endptr = (char *)&str[i];
        	return (0);
		}
    *endptr = (char *)&str[ft_strlen(str)];
    return (res * sign);
}

static int	check_llong(char *str)
{
    char *endptr;

	if (ft_strlen(str) > 20)
		return (1);
    ft_strtoll(str, &endptr, 10);
	if (*endptr != '\0')
		return (1);
	return (0);
}

/**
 * @brief Exit the shell with a status code (default 0)
 *
 * @param t_cmd *cmd Command structure
 * @return int Exit code
 */
int	command_exit(t_cmd *cmd)
{
	int	status;

	status = 0;
	if (check_llong(cmd->args[1]) || str_is_digit(cmd->args[1]) == 0)
    {
        ft_fprintf(2, "minishell: exit: %s: numeric argument required\n",
            cmd->args[1]);
        cmd->exit_signal = 1;
        return (2);
    }
	if (cmd->argc > 2)
	{
		ft_fprintf(2, "minishell: exit: too many arguments\n");
		return (1);
	}
	else if (cmd->argc > 1)
		status = ft_atoi(cmd->args[1]);
	cmd->exit_signal = 1;
	return (status % 256);
}
