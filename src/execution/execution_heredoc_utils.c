/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_heredoc_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <pierrot.maxime.pro@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 01:18:56 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/24 01:18:56 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	bite_heredoc(t_heredoc_info *heredoc_info, int i)
{
	ssize_t	bites;

	bites = write(heredoc_info->pipes[1],
			heredoc_info->texts[i], ft_strlen(heredoc_info->texts[i]));
	if (bites == -1)
	{
		ft_fprintf(STDERR_FILENO,
			"Error 1 on fd %d: write failed\n", heredoc_info->pipes[1]);
		ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
		return (0);
	}
	bites = write(heredoc_info->pipes[1], "\n", 1);
	if (bites == -1)
	{
		ft_fprintf(STDERR_FILENO,
			"Error 2 on fd %d: write failed\n", heredoc_info->pipes[1]);
		ft_putstr_fd("Error: write failed\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

/**
 * @brief Write the heredoc content to the pipe
 *
 * @param t_heredoc_info *heredoc_info Information sur le heredoc
 * @return int 1 on success, 0 on failure
 */
int	write_heredoc(t_heredoc_info *heredoc_info)
{
	int		i;

	i = 0;
	while (heredoc_info->texts[i++])
	{
		if (ft_strlen(heredoc_info->texts[i]) == 0)
			continue ;
		if (!bite_heredoc(heredoc_info, i))
			return (0);
	}
	return (1);
}

int	read_heredoc(t_heredoc_info *heredoc_info)
{
	int		i;
	char	*line;

	i = 0;
	while (1)
	{
		line = gnl_heredoc(heredoc_info, i);
		if (!line)
			return (0);
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

char	*gnl_heredoc(t_heredoc_info *heredoc_info, int i)
{
	char	*tmp;
	char	*line;

	ft_putstr_fd("> ", STDOUT_FILENO);
	tmp = get_next_line(STDIN_FILENO);
	line = ft_strtrim(tmp, "\n");
	free(tmp);
	if (!line)
	{
		ft_fprintf(STDERR_FILENO,
			"minishell: warning: heredoc at line %d"
			"delimited by end-of-file (wanted `%s`)\n",
			i, heredoc_info->delimiter);
		return (NULL);
	}
	return (line);
}
