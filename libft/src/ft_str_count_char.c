/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_count_char.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:06:06 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:06:10 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//count number of int c in string str.
size_t	ft_str_count_char(char const *str, int c)
{
	size_t	res;

	res = 0;
	while (*str)
	{
		if (*str == (char)c)
			res++;
		str++;
	}
	return (res);
}
