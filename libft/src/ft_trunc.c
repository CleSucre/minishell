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

void	ft_trunc(char **str, size_t era)
{
	size_t	len;

	len = ft_strlen(*str);
	if (len < era)
		return ;
	while (era)
	{
		(*str)[len - era] = '\0';
		era--;
	}
}
