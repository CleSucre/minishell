/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:56:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/16 00:36:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief List the files in the directory
 *
 * @param char *path The path to the directory to load
 * @return char** The list of files in the directory
 */
static char	**list_dir(char *path)
{
	DIR		*dir;
	char	**files;
	struct	dirent *dirent;

	files = NULL;
	if (access(path, R_OK) == -1)
		return (NULL);
	dir = opendir(path);
	dirent = readdir(dir);
	while (dirent)
	{
		ft_tabadd(&files, dirent->d_name);
		dirent = readdir(dir);
	}
	closedir(dir);
	return (files);
}

/**
 * @brief Memory swap two pointers
 *
 * @param void **a The first pointer
 * @param void **b The second pointer
 */
static void	ft_memswap(void **a, void **b)
{
	void	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/**
 * @brief Sort the files in alphabetical order
 *
 * @param char ***files The list of files to sort
 */
static void alphaetical_sort(char ***files)
{
	int	i;

	i = 0;
	while ((*files)[i + 1])
	{
		if (ft_strcmp((*files)[i], (*files)[i + 1]) > 0)
		{
			ft_memswap((void **)&(*files)[i], (void **)&(*files)[i + 1]);
			i = 0;
		}
		else
			i++;
	}
}

/**
 * @brief Load the files in the current directory
 * 			if the files are not already loaded.
 *
 * @param char ***files
 */
static void	load_files_if_null(char ***files)
{
	char *path;

	if (*files != NULL)
		return ;
	path = getcwd(NULL, 0);
	*files = list_dir(path);
	alphaetical_sort(files);
	free(path);
}

char *ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!needle[0])
		return ((char *)haystack);
	while (haystack[i])
	{
		if (haystack[i] == needle[j])
		{
			while (haystack[i + j] == needle[j] && needle[j])
				j++;
			if (!needle[j])
				return ((char *)&haystack[i]);
			j = (0);
		}
		i++;
	}
	return (NULL);
}

static int	match_wildcard(const char *pattern, const char *str)
{
	if (*str == '.' && *pattern != '.')
		return (0);
	while (*pattern && *str)
	{
		if (*pattern == '*')
		{
			pattern++;
			if (!*pattern)
				return (1);
			while (*str)
			{
				if (match_wildcard(pattern, str))
					return (1);
				str++;
			}
			return (0);
		}
		else if (*pattern == '?' || *pattern == *str)
		{
			pattern++;
			str++;
		}
		else
			return (0);
	}
	while (*pattern == '*')
		pattern++;
	return (!*pattern && !*str);
}

static int	count_matches(char *search, char **files)
{
	int count = 0;
	int i = 0;

	while (files[i])
	{
		if (match_wildcard(search, files[i]))
			count++;
		i++;
	}
	return (count);
}

static char	**expand_wildcard(char **str, char **files)
{
	char	**match;
	int		i;
	int		j;
	int		match_count;
	char 	*search;

	search = *str;
	match_count = count_matches(search, files);
	match = (char **)malloc(sizeof(char *) * (match_count + 1));
	if (!match)
		return (NULL);

	i = 0;
	j = 0;
	while (files[i])
	{
		if (match_wildcard(search, files[i]))
		{
			match[j] = strdup(files[i]);
			j++;
		}
		i++;
	}
	match[j] = NULL;
	return (match);
}

/**
 * @brief Support wildcard expansion.
 * 			Replace the wildcard with the list of
 * 			files matching it only if there is a match.
 * 			Search the files in the current directory.
 *
 * @param char **args The arguments to expand
 * @param t_minishell *minishell
 */
void expand_wildcards(t_cmd *cmd)
{
	int		i;
	char 	**files;
	char 	**match;
	char 	**new_args;

	files = NULL;
	new_args = NULL;
	i = 0;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '*'))
		{
			load_files_if_null(&files);
			match = expand_wildcard(&cmd->args[i], files);
			if (match)
			{
				if (!new_args)
				{
					new_args = ft_tabdup((const char **)match);
					ft_tabfree(match);
				}
				else
					new_args = ft_tabjoin(new_args, match);
			}
		}
		else
			ft_tabadd(&new_args, cmd->args[i]);
		i++;
	}
	if (files)
		ft_tabfree(files);
	if (new_args)
	{
		ft_tabfree(cmd->args);
		cmd->args = new_args;
	}
}
