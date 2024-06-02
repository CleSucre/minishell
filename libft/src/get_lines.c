/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_lines.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 03:34:10 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/29 03:34:10 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**get_lines(int fd)
{
	char	*buffer;
	char	*file_content;
	int		bytes_read;
	char	**lines;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	file_content = NULL;
	bytes_read = BUFFER_SIZE;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		buffer[bytes_read] = '\0';
		ft_realloc(&file_content, buffer);
	}
	if (bytes_read < 0)
	{
		free(file_content);
		return (NULL);
	}
	lines = ft_split(file_content, "\n");
	free(file_content);
	free(buffer);
	return (lines);
}
