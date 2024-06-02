/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 20:04:00 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/07 20:04:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static void	try_free(char **readed, char **tmp)
{
	free(*readed);
	*readed = *tmp;
	if (ft_strlen(*readed) == 0)
	{
		free(*readed);
		*readed = NULL;
	}
}

static char	*ft_get_line(char *readed)
{
	char	*res;
	size_t	i;

	i = 0;
	while (readed[i] && readed[i] != '\n')
		i++;
	if (i == 0 && readed[i] != '\n')
		return (NULL);
	if (readed[i] == '\n')
		i++;
	res = (char *)malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (readed[i] && readed[i] != '\n')
	{
		res[i] = readed[i];
		i++;
	}
	if (readed[i] == '\n')
		res[i++] = '\n';
	res[i] = '\0';
	return (res);
}

static void	ft_clean_readed(char **readed)
{
	char	*tmp;
	size_t	i;
	size_t	j;

	i = 0;
	while ((*readed)[i] && (*readed)[i] != '\n')
		i++;
	if ((*readed)[i] == '\0')
	{
		free(*readed);
		*readed = NULL;
		return ;
	}
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(*readed) - i + 1));
	if (!tmp)
		return ;
	i++;
	j = 0;
	while ((*readed)[i])
		tmp[j++] = (*readed)[i++];
	tmp[j] = '\0';
	try_free(readed, &tmp);
}

static int	ft_strcontain(const char *s, int c)
{
	int	i;

	if (s == NULL)
		return (0);
	i = 0;
	while (s[i])
	{
		if ((char)c == s[i])
			return (1);
		i++;
	}
	if ((char)c == s[i])
		return (1);
	return (0);
}

char	*get_next_line(int fd)
{
	static char	*readed[4096];
	char		*buffer;
	int			bytes_read;
	char		*res;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = BUFFER_SIZE;
	while (!ft_strcontain(readed[fd], '\n') && bytes_read == BUFFER_SIZE)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == 0 || bytes_read == -1)
			break ;
		buffer[bytes_read] = '\0';
		ft_realloc(&readed[fd], buffer);
	}
	free(buffer);
	if (readed[fd] == NULL)
		return (NULL);
	res = ft_get_line(readed[fd]);
	ft_clean_readed(&readed[fd]);
	return (res);
}
