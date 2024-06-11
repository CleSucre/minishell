/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_tgetent.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 05:39:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/06/03 05:39:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

int my_tgetent(t_term *info, const char *term_type)
{
	int fd;
	char buffer[4096];
	ssize_t bytes_read;
	char *line_start = buffer;
	char *line_end;
	int found;

	found = 0;
	fd = open("/etc/termcap", O_RDONLY);
	if (fd == -1)
	{
		ft_fprintf(2, "A problem with file opening occurs");
		return (-1);
	}
	bytes_read = read(fd, buffer, 4096);
	while (bytes_read > 0)
	{
		line_end = buffer;
		while ((line_end = ft_memchr(line_start, '\n', buffer + bytes_read - line_start)))
		{
			*line_end = '\0';
			if (ft_strncmp(line_start, term_type, ft_strlen(term_type)) == 0 &&
				line_start[ft_strlen(term_type)] == '|')
			{
				found = 1;
				ft_strncpy(info->term_name, term_type, sizeof(info->term_name) - 1);
				ft_strncpy(info->capabilities, line_start, sizeof(info->capabilities) - 1);
				break;
			}
			line_start = line_end + 1;
		}
		if (found)
			break;
		if (line_start < buffer + bytes_read)
		{
			ft_memmove(buffer, line_start, buffer + bytes_read - line_start);
			bytes_read -= line_start - buffer;
			line_start = buffer + bytes_read;
		}
		else
		{
			line_start = buffer;
			bytes_read = 0;
		}
		bytes_read = read(fd, buffer, 4096);
	}
	close(fd);
	if (!found)
	{
		ft_fprintf(2, "Terminal type %s not found \n", term_type);
		return (0);
	}
	return (1);
}