/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/14 19:35:36 by mpierrot         ###   ########.fr       */
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
//char	*symlink_to_path(char *symlink)
//{
//	char	*dest;
//	(void)symlink;
//	(void)dest;
//
//	return (dest);
//}

/**
 * @brief Check if the path linked to by symlink exist and is accessible
 * @param symlink
 * @return 0 if success, 1 if not
 */
//int is_symlink_accessible(char *symlink)
//{
//	int res;
//	(void)symlink;
//	(void)res;
//	return (res);
//}

/**
 * @brief Check if dir exist and if we have access to it
 * @param dir
 * @return 0 if success, 1 if not
 */
//int cd_error_handler(char *dir)
//{
//	(void)dir;
//		if (access(dir, X_OK) == 0)
//			return (1);
//		return (0);
//}

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

void	clear_string(char *str)
{
	int	len;
	int i;

	len = ft_strlen(str);
	i = 0;
	while(i < len)
	{
		str[i] = '\0';
		i++;
	}
}

/**
 * @brief find a specific lines in tab about search param
 * 			Example : in an env tab, find specific string about PATH
 * @param table
 * @param search
 * @return i if success, -1 if failed
 */
int	find_args(char **table, char *search)
{
	int i;

	i = 0;
	while(table[i])
	{
		if (ft_strncmp(table[i], search, ft_strlen(search)) == 0)
			return (i) ;
		i++;
	}
	return (-1);
}

/**
 * @brief copy str in dst passing nb char
 * @param str string to cpy
 * @param dst result
 * @param nb number of char to pass
 * @return
 */
char	*strcpy_passing_char(char *str, int nb)
{
	int		i;
	char 	*dst;

	dst = ft_calloc(sizeof(char *) * ft_strlen(str) - nb, 1);
	if (!dst)
		return (NULL);
	i = 0;
	while(str[nb])
	{
		dst[i] = str[nb];
		i++;
		nb++;
	}
	return (dst);
}

/**
 * @brief function to invert olwdpwd and pwd when cd - is used
 * @param minishell
 * @param search
 */
static void	invert_oldpwd(t_minishell *minishell)
{
	int 	pwd;
	char 	*name;
	int 	oldpwd;
	char 	buffer[BUFFER_SIZE];

	oldpwd = find_args(minishell->env, "OLDPWD");
	pwd = find_args(minishell->env, "PWD");//					Search tabs pwd && oldpwd
	ft_fprintf(2, "oldpwd [%s], pwd [%s]\n\n\n\n", minishell->env[oldpwd], minishell->env[pwd]);
	if (oldpwd == -1 || pwd == -1)
		return ;
	name = strcpy_passing_char(minishell->env[oldpwd], 7);
	if (chdir(name) == -1)//											go to oldpwd dir
		ft_putstr_fd("Error accessing oldpwd\n", 2);
	free(name);
	clear_string(minishell->env[oldpwd]);//								change oldpwd name to pwd name
	ft_strlcpy(minishell->env[oldpwd], ft_strjoin("OLD", minishell->env[pwd]), (int)ft_strlen(minishell->env[pwd]) + 4);
	clear_string(minishell->env[pwd]);//								clean pwd name
	name = getcwd(buffer, BUFFER_SIZE);//							get new name about cwd
	ft_strlcpy(minishell->env[pwd], ft_strjoin("PWD=", name), (int)ft_strlen(name) + 5);
	ft_fprintf(2, "AFTER oldpwd [%s], pwd [%s]\n", minishell->env[oldpwd], minishell->env[pwd]);
	return ;
}


//getcwd, chdir, getcwd


static void	change_oldpwd(t_minishell *minishell)
{
	int 	pwd;
	char 	*name;
	int 	oldpwd;
	char 	buffer[BUFFER_SIZE];

	oldpwd = find_args(minishell->env, "OLDPWD");
	pwd = find_args(minishell->env, "PWD");
	if (oldpwd == -1 || pwd == -1)
	{
		ft_putstr_fd("not find\n", 2);
		return ;
	}
	ft_fprintf(2, "BEFORE CHANGE\noldpwd [%s], pwd [%s]\n\n\n\n", minishell->env[oldpwd], minishell->env[pwd]);	
	clear_string(minishell->env[oldpwd]);
	ft_strlcpy(minishell->env[oldpwd], ft_strjoin("OLD", minishell->env[pwd]), (int)ft_strlen(minishell->env[pwd]) + 4);
	clear_string(minishell->env[pwd]);
	name = getcwd(buffer, BUFFER_SIZE);
	ft_strlcpy(minishell->env[pwd], ft_strjoin("PWD=", name), (int)ft_strlen(name) + 5);
	ft_fprintf(2, "AFTER CHANGE\noldpwd [%s], pwd [%s]\n", minishell->env[oldpwd], minishell->env[pwd]);
	return ;
}
//Ancien - > avant chgmt
//Nvx -> apres chgmt

/**
 * @brief Change the current working directory
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
	(void)buffer;

	if (ft_strcmp(cmd->argv[1], "-") == 0)
	{
		if (access(getenv("OLDPWD"), R_OK | X_OK) == 0)
			invert_oldpwd(minishell);
		else
		{
			ft_putstr_fd("error, oldpwd can't be established\n", 2);
			minishell->exit_code = 126;
			return (1);
		}
	}
	else if (chdir(cmd->argv[1]) == -1 || access(cmd->argv[1], R_OK | X_OK) != 0)
	{
		ft_fprintf(2, "minishell: cd: %s: No such file or directory\n", cmd->argv[1]);
		minishell->exit_code = 1;
		return (1);
	}
	else
		change_oldpwd(minishell);
	minishell->exit_code = 0;
//	change_oldpwd(minishell, "OLDPWD=");
//	minishell->dirinfo->old_path = minishell->dirinfo->path;
//	minishell->dirinfo->path = getcwd(buffer, BUFFER_SIZE);
	return (0);
}
