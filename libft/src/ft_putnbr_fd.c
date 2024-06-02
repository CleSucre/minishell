/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:05:45 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:05:47 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int nb, int fd)
{
	int		res;
	long	n;

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
