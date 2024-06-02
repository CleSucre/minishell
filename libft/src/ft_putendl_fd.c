/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:04:46 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:05:33 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putendl_fd(char *s, int fd)
{
	int	res;

	if (!s)
		return (0);
	res = 0;
	res += ft_putstr_fd(s, fd);
	res += ft_putchar_fd('\n', fd);
	return (res);
}
