/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dtoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:03:09 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:03:14 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_insert(char *dst, long n, int *i)
{
	if (n > 9)
	{
		ft_insert(dst, n / 10, i);
		ft_insert(dst, n % 10, i);
	}
	else
		dst[(*i)++] = n + '0';
}

static int	ft_topositive(char *dst, long *num, double *n)
{
	if (*n < 0)
	{
		dst[0] = '-';
		*num = -*num;
		*n = -*n;
		return (1);
	}
	return (0);
}

char	*ft_dtoa(double n, int precision)
{
	char	*res;
	long	num;
	int		i;

	num = (long)n;
	n = n - num;
	res = (char *)malloc(sizeof(char) * (ft_nbrlen(num) + 1 + precision + 1));
	if (!res)
		return (NULL);
	i = ft_topositive(res, &num, &n);
	ft_insert(res, num, &i);
	res[i++] = '.';
	while (precision--)
	{
		n *= 10;
		num = (long)n;
		res[i++] = num + '0';
		n -= num;
	}
	res[i] = '\0';
	return (res);
}
