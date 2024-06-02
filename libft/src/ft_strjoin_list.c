/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:06:24 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:06:26 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_list(char const **strs)
{
	int		i;
	char	*res;

	i = 0;
	while (strs[i])
	{
		res = ft_strjoin(res, strs[i++]);
		if (!res)
			return (NULL);
	}
	return (res);
}
