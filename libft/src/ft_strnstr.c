/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:06:52 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:06:54 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	ii;

	if (!haystack && len <= 0)
		return (NULL);
	if (!*needle)
		return ((char *)haystack);
	i = -1;
	while (haystack[++i] && i < len)
	{
		ii = 0;
		if (haystack[i] == needle[ii])
		{
			while (needle[ii] && i + ii < len)
			{
				if (haystack[i + ii] != needle[ii])
					break ;
				if (!needle[ii++ + 1])
					return ((char *)haystack + i);
			}
		}
	}
	return (NULL);
}
