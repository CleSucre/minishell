/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*wait_input(t_minishell *minishell, const char *prompt)
{
	char	*line;

	ft_printf(BOLDWHITE"%s "RESET, prompt);
	line = get_next_line(0);
	if (!line)
	{
		if (DEBUG)
			ft_fprintf(2, BOLDRED"Error: "RESET"get_next_line failed\n");
		return (NULL);
	}
	line[ft_strlen(line) - 1] = '\0';
	return (line);
}

int	exec_command(t_minishell *minishell, char *input)
{
	if (DEBUG)
		ft_printf(BOLDWHITE"[DEBUG] command: "RESET"\"%s\"\n", input);
	if (ft_strlen(input) != 0)
		add_to_history(minishell, input);
	if (ft_strcmp(input, "exit") == 0)
	{
		free(input);
		return (1);
	}
	free(input);
	return (0);
}
