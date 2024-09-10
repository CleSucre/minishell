/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 06:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** https://github.com/bminor/bash/blob/master/builtins/cd.def
 * @brief Print the current working directory
 *
 * @param cmd The command structure
 *
 * @info
 * from bash source code :
 * static int bindpwd PARAMS((int));
 *	static int setpwd PARAMS((char *));
 *	static char *resetpwd PARAMS((char *));
 *	static int change_to_directory PARAMS((char *, int, int));
 *	@return 0 if success, 1 if fail
 */

void	command_pwd(t_cmd *cmd)
{
	(void)cmd;
	char *path;
	char buffer[BUFFER_SIZE];

	path = getcwd(buffer, BUFFER_SIZE);
	if (!path)
	{
		ft_fprintf(2, "Cannot get current directory path\n");
		if (errno == ERANGE)
			ft_fprintf(2, "Buffer size is too small\n");
		exit(EXIT_FAILURE);
	}
	else
		ft_fprintf(1, "%s\n", path);
}
