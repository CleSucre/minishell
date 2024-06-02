/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_float_fd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 09:25:49 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/22 09:25:49 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_float_fd(double n, int fd)
{
	int		res;
	int		i;
	double	dec;

	res = 0;
	i = 0;
	if (0 > n)
	{
		res += ft_putchar_fd('-', fd);
		n *= -1;
	}
	res += ft_putnbr_fd((int)n, fd);
	res += ft_putchar_fd('.', fd);
	n -= (int)n;
	while (i < 6)
	{
		n *= 10;
		dec = (int)n;
		res += ft_putnbr_fd((int)dec, fd);
		n -= dec;
		i++;
	}
	return (res);
}
