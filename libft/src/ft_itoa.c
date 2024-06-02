/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
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

char	*ft_itoa(int n)
{
	char	*res;
	long	num;
	int		i;

	num = n;
	res = (char *)malloc(sizeof(char) * (ft_nbrlen(num) + 1));
	if (!res)
		return (NULL);
	i = 0;
	if (num < 0)
	{
		res[i++] = '-';
		num *= -1;
	}
	ft_insert(res, num, &i);
	res[i] = '\0';
	return (res);
}
