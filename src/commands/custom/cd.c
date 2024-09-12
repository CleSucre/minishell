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
 *  Notes :
 *
 *  function use from bash :
 *
 *  static int bindpwd PARAMS((int));								V
 *	static int setpwd PARAMS((char *)); 							V
 *	static char *resetpwd PARAMS((char *));							V
 *	static int change_to_directory PARAMS((char *, int, int));		V
 *	static int cdxattr PARAMS((char *, char **));					X
 *	static void resetxattr PARAMS((void));							X
 *
 *
 *	resolve symbolic links in DIR before
 *		processing instances of `..'
 *
 *		 * Might use chdir, getcwd, access, stat,
 *
 *   chdir  changes  the current directory to that specified in
 *      path.
 *   getcwd() returns an absolute pathname of the current working directory.
 *   access() checks whether the calling process can access the file pathname.
 *   stat() stats the file pointed to by pathname and fills in buf.
 *   lstat() is identical to stat(), except that if pathname is a symbolic link, then it stats the symbolic link itself, not the file that it refers to.
 *  fstat() is identical to stat(), except that the file about which information is to be retrieved is specified by the file descriptor fd.
 *
 *  @info Cd sans rien a cote ramene au repertoire personnel : /home/user
 *  cd - ramene au repertoire precedent -> oldpwd
 *  cd ~ ramene au repertoire personnel
 *  cd ~user ramene au repertoire personnel de l'utilisateur user
 *  cd .. ramene au repertoire parent
 *  cd / ramene a la racine
 *  cd /chemin/absolu ramene au chemin absolu
 *  cd ./chemin/relatif ramene au chemin relatif
 *  Si dossier existe pas, erreur
 *  si pas les permissions, erreur
 * The default DIR is the value of the
 * HOME shell variable. If DIR is "-", it is converted to $OLDPWD
 * `..' is processed by removing the immediately previous pathname component
 * back to a slash or the beginning of DIR.
 *
 *  cd [-L|[-P [-e]] [-@]] [dir]
 *
 * flags utils
 *
 *
 *  Set oldpwd then change pwd
 */



/**
 * @brief Give the real path linked to by symlink
 * @param symlink
 * @return path if success, NULL if not
 */
char	*symlink_to_path(char *symlink)
{
	char	*dest;

	return (dest);
}

/**
 * @brief Check if the path linked to by symlink exist and is accessible
 * @param symlink
 * @return 0 if success, 1 if not
 */
int is_symlink_accessible(char *symlink)
{
	int res;

	return (res);
}

/**
 * @brief Check if dir exist and if we have access to it
 * @param dir
 * @return 0 if success, 1 if not
 */
int cd_error_handler(char *dir)
{
		if (access(dir, X_OK) == 0)
			return (1);
		return (0);
}

/**
 * @brief Check if the path given is an absolute path
 * @param path
 * @return 0 if yes, 1 if not
 */
int is_absolute_path(char *path)
{
	if(path[0] == 47)
		return (0);
	return (1);
}


/**
 * @brief Change the current working directory.
 *		Only with relative or absolute path
 *		The relative path of a file is its location relative to the present working directory.
 *			It never starts with a slash (/). It begins with the continued work directory.
 * @param cmd The command to execute.
 * By default, as if -L is used
 * @return int 0 if the command was executed successfully, 1 otherwise.
 */
int	command_cd(t_minishell *minishell, t_cmd *cmd)
{
	char	buffer[BUFFER_SIZE];

	(void)cmd;
	if (ft_strcmp(cmd->argv[1], "-") == 0)
	{
		if (getenv("OLDPWD"))
			chdir(getenv("OLDPWD"));
		else
			ft_putstr_fd("error, oldpwd can't be established", 2);
	}
	else
		chdir(cmd->argv[1]);
	minishell->dirinfo->old_path = minishell->dirinfo->path;
	minishell->dirinfo->path = getcwd(buffer, BUFFER_SIZE);
	return (0);
}
