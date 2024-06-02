/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putpointer_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:21:35 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/10 18:42:50 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putpointer_fd(void *ptr, int fd)
{
	int	count;

	if (ptr == NULL)
		return (ft_putstr_fd("(nil)", fd));
	count = 0;
	count += ft_putstr_fd("0x", fd);
	count += ft_putnbr_base_fd((unsigned long long)ptr, "0123456789abcdef", fd);
	return (count);
}
