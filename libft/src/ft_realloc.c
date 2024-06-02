/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 03:33:56 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/29 03:33:56 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_realloc(char **s1, const char *s2)
{
	char	*tmp;
	int		i;
	int		j;

	if (*s1 == NULL)
	{
		*s1 = (char *)malloc(sizeof(char) * 1);
		if (*s1 == NULL)
			return ;
		**s1 = '\0';
	}
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(*s1) + ft_strlen(s2) + 1));
	if (!tmp)
		return ;
	i = -1;
	while ((*s1)[++i])
		tmp[i] = (*s1)[i];
	j = 0;
	while (s2[j])
		tmp[i++] = s2[j++];
	tmp[i] = '\0';
	free(*s1);
	*s1 = tmp;
}
