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
 */

void	command_pwd(t_cmd *cmd)
{
	char *path;

	path = getenv("PWD");
	ft_printf("Le path : [%s]\n", path);
	(void)cmd;
}
