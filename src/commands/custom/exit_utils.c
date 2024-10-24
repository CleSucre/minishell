/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:18:06 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/22 21:24:48 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Convert a string to a long long integer
 *
 * @param str String to convert
 * @param endptr Pointer to the character that stops the conversion
 * @param base Base of the number
 * @return long long Converted number
 */
static int	check_digit(char *str, char **endptr, int base, long long *res)
{
	int	i;
	int	digit;

	i = 0;
	while (ft_isdigit(str[i]))
	{
		digit = str[i] - '0';
		if (*res > (LLONG_MAX - digit) / base)
		{
			*endptr = (char *)&str[i];
			return (i);
		}
		*res = *res * base + digit;
		i++;
	}
	return (0);
}

long long	ft_strtoll(const char *str, char **endptr, int base)
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
	if (check_digit((char *)&str[i], endptr, base, &res))
	{
		*endptr = (char *)&str[i];
		return (0);
	}
	*endptr = (char *)&str[ft_strlen(str)];
	return (res * sign);
}

static char	*copy(char *tmp, size_t a, size_t i)
{
	while (tmp[i])
	{
		tmp[a] = tmp[i];
		a++;
		i++;
	}
	tmp[a] = '\0';
	return (tmp);
}

char	*noptozero(char *tmp)
{
	size_t	i;
	size_t	a;

	i = 0;
	a = 0;
	if ((tmp[0] == 43 || tmp[0] == 45) && tmp[1] && tmp[1] != ' ')
	{
		a++;
		i++;
	}
	while (tmp[i] == '0')
		i++;
	if (!tmp[i])
	{
		tmp[0] = '0';
		tmp[1] = '\0';
		return (tmp);
	}
	tmp = copy(tmp, a, i);
	return (tmp);
}
