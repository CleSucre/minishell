//
// Created by Julie on 02/06/2024.
//

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
	while(str[i])
	{
		str[i] = '\0';
		i++;
	}
}