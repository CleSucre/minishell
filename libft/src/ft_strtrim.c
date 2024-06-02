/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:07:04 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:07:05 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		start;
	int		i;
	int		ii;

	if (!s1)
		return (NULL);
	start = -1;
	while (s1[++start])
		if (!ft_strchr(set, s1[start]))
			break ;
	i = ft_strlen(s1);
	while (s1[--i])
		if (!ft_strchr(set, s1[i]))
			break ;
	i = i - start + 1;
	if (i < 0)
		i = 0;
	res = (char *)ft_calloc(sizeof(char), (i + 1));
	if (!res)
		return (NULL);
	ii = 0;
	while (i--)
		res[ii++] = s1[start++];
	return (res);
}
