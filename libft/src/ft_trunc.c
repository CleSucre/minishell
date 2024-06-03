/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trunc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 05:31:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/03 05:31:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_trunc(char *str, size_t era)
{
	size_t	len;
	int		i;

	len = ft_strlen(str);
	if (len < era)
		return ;
	i = len - era - 1;
	if (i < 0)
		i = 0;
	while (str[i])
	{
		str[i] = '\0';
		i++;
	}
}
