/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:05:41 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:05:42 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_base_fd(int nbr, char *base, int fd)
{
	long	i;
	int		size;
	int		temp;

	size = ft_strlen(base);
	i = nbr;
	if (i < 0)
	{
		ft_putchar_fd('-', fd);
		i *= -1;
	}
	if (i < size)
	{
		ft_putchar_fd(base[i], fd);
		return (1);
	}
	else
	{
		temp = 0;
		temp += ft_putnbr_base_fd(i / size, base, fd);
		temp += ft_putnbr_base_fd(i % size, base, fd);
	}
	return (temp);
}
