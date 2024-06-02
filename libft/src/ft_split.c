/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:05:52 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/02 17:05:54 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_free(char **tab, int i)
{
	while (i >= 0)
		free(tab[i--]);
	free(tab);
	return (1);
}

static int	ft_exec(char **dst, int size, char const *str, char const *charset)
{
	int	i;
	int	ii;
	int	start;

	i = 0;
	ii = 0;
	while (size-- > 0)
	{
		while (ft_str_count_char(charset, str[i]) != 0)
			i++;
		start = i;
		while (ft_str_count_char(charset, str[i]) == 0 && str[i])
			i++;
		dst[ii] = ft_substr(str, start, i - start);
		if (!dst[ii])
			return (ft_free(dst, ii));
		ii++;
	}
	dst[ii] = NULL;
	return (0);
}

char	**ft_split(char const *str, char const *charset)
{
	char	**res;
	int		size;

	if (!str || !charset)
		return (NULL);
	size = ft_count_words(str, charset);
	res = (char **)malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (NULL);
	if (ft_exec(res, size, str, charset) == 1)
		return (NULL);
	return (res);
}
