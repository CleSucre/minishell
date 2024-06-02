/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 03:48:19 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/22 03:48:19 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_count_words(char const *str, char const *charset)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (ft_str_count_char(charset, str[i]) == 0)
		{
			count++;
			while (ft_str_count_char(charset, str[i]) == 0 && str[i])
				i++;
		}
		else
			i++;
	}
	return (count);
}
