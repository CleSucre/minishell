/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_heredoc_pid.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <pierrot.maxime.pro@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 01:22:26 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/24 01:22:26 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "global.h"

int	write_line_to_pipe(int fd, const char *line)
{
	ssize_t	bites;

	bites = write(fd, line, ft_strlen(line));
	if (bites == -1)
	{
		ft_fprintf(STDERR_FILENO, "Error on fd %d: write failed\n", fd);
		ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
		return (0);
	}
	bites = write(fd, "\n", 1);
	if (bites == -1)
	{
		ft_fprintf(STDERR_FILENO, "Error on fd %d: write failed\n", fd);
		ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	write_heredoc(t_heredoc_info *heredoc_info)
{
	int	i;

	i = 0;
	while (heredoc_info->texts[i])
	{
		if (ft_strlen(heredoc_info->texts[i]) == 0)
		{
			i++;
			continue ;
		}
		if (!write_line_to_pipe(heredoc_info->pipes[1],
				heredoc_info->texts[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*begin_read(void)
{
	char	*tmp;
	char	*line;

	ft_putstr_fd("> ", STDOUT_FILENO);
	tmp = get_next_line(STDIN_FILENO);
	line = ft_strtrim(tmp, "\n");
	free(tmp);
	return (line);
}

int	read_heredoc(t_heredoc_info *heredoc_info)
{
	int		i;
	char	*line;

	i = 0;
	while (1)
	{
		line = begin_read();
		if (!line)
		{
			ft_fprintf(STDERR_FILENO, "minishell: warning: heredoc at line %d"
				"delimited by end-of-file (wanted `%s`)\n",
				i, heredoc_info->delimiter);
			return (0);
		}
		if (ft_strcmp(line, heredoc_info->delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_tabadd(&heredoc_info->texts, line);
		free(line);
		i++;
	}
	return (1);
}
