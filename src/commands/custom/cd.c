/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/16 06:52:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** https://github.com/bminor/bash/blob/master/builtins/cd.def
 * @brief Change the current working directory.
 *
 * @param cmd The command to execute.
 *
 * Might use chdir, getcwd, access, stat,
 *
 *   chdir  changes  the current directory to that specified in
       path.
 *   getcwd() returns an absolute pathname of the current working directory.
 *   access() checks whether the calling process can access the file pathname.
 *   stat() stats the file pointed to by pathname and fills in buf.
 *   lstat() is identical to stat(), except that if pathname is a symbolic link, then it stats the symbolic link itself, not the file that it refers to.
 *  fstat() is identical to stat(), except that the file about which information is to be retrieved is specified by the file descriptor fd.
 *
 *  @info Cd sans rien a cote ramene au repertoire personnel : /home/user
 *  cd - ramene au repertoire precedent
 *  cd ~ ramene au repertoire personnel
 *  cd ~user ramene au repertoire personnel de l'utilisateur user
 *  cd . ramene au repertoire courant
 *  cd .. ramene au repertoire parent
 *  cd / ramene a la racine
 *  cd /chemin/absolu ramene au chemin absolu
 *  cd ./chemin/relatif ramene au chemin relatif
 *  Si dossier existe pas, erreur
 *  si pas les permissions, erreur
 *
 *  cd [-L|[-P [-e]] [-@]] [dir]
 *
 * flags utils
 *
 * @return int 0 if the command was executed successfully, 1 otherwise.
 */


int	command_cd(t_cmd *cmd)
{
	(void)cmd;
	return (0);
}
