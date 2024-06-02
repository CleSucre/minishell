/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:04:23 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:04:26 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*res;
	const char	*cpy;
	size_t		i;

	if (dst == NULL && src == NULL)
		return (dst);
	res = dst;
	cpy = src;
	if (dst > src)
	{
		while (len)
		{
			len--;
			res[len] = cpy[len];
		}
		return (dst);
	}
	i = 0;
	while (i < len)
	{
		res[i] = cpy[i];
		i++;
	}
	return (dst);
}
