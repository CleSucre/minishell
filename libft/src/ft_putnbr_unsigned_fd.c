/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_unsigned_fd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:02:25 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/10 18:42:48 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_unsigned_fd(unsigned int nb, int fd)
{
	int			res;
	long long	n;

	res = 0;
	n = nb;
	if (n < 0)
	{
		res++;
		ft_putchar_fd('-', fd);
		n = -n;
	}
	if (n >= 10)
		res += ft_putnbr_fd(n / 10, fd);
	res += ft_putchar_fd(n % 10 + '0', fd);
	return (res);
}
