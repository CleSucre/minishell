/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:06:30 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:06:32 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	ii;

	if (!dstsize)
		return (ft_strlen(src));
	i = 0;
	while (dst[i] && i < dstsize)
		i++;
	if (dstsize == 0)
		return (i + ft_strlen(src));
	ii = i;
	while (src[i - ii] && i < dstsize - 1)
	{
		dst[i] = src[i - ii];
		i++;
	}
	if (ii < dstsize)
		dst[i] = '\0';
	return (ii + ft_strlen(src));
}
