/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_count_chars.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:06:13 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:06:14 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//count the number of chars in string s1 corresponding to chars in s2.
size_t	ft_str_count_chars(char const *s1, char const *s2)
{
	size_t	res;

	res = 0;
	while (*s1)
	{
		if (ft_str_count_char(s2, *s1))
			res++;
		s1++;
	}
	return (res);
}
