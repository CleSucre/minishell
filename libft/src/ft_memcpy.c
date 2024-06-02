/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:04:18 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:04:21 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*res;
	const char	*cpy;

	if (dst == NULL && src == NULL)
		return (dst);
	res = dst;
	cpy = src;
	while (n--)
	{
		*res = *cpy;
		res++;
		cpy++;
	}
	return (dst);
}
