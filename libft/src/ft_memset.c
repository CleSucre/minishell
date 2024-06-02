/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:04:30 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:04:33 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	size_t			word;

	ptr = b;
	word = (unsigned char)c;
	word = (word << 8) | word;
	word = (word << 16) | word;
	if (sizeof(size_t) == 8)
		word = (word << 32) | word;
	while (len >= sizeof(size_t))
	{
		*(size_t *)ptr = word;
		ptr += sizeof(size_t);
		len -= sizeof(size_t);
	}
	while (len > 0)
	{
		*ptr = (unsigned char)c;
		ptr++;
		len--;
	}
	return (b);
}
