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
 * @brief Load the files in the current directory
 * 			if the files are not already loaded.
 *
 * @param char ***files
 */
void	load_files_if_null(char ***files)
{
	char	*path;

	if (*files != NULL)
		return ;
	path = getcwd(NULL, 0);
	*files = ft_list_dir(path);
	ft_sort(*files, 0, ft_tablen((const char **)*files) - 1);
	free(path);
}

/**
 * @brief Helper function for matching when a wildcard '*' is found
 *
 * @param const char *pattern The wildcard pattern
 * @param const char *str The string to match
 * @return int 1 if the string matches, 0 otherwise
 */
int	handle_wildcard(const char *pattern, const char *str)
{
	while (*str)
	{
		if (match_wildcard(pattern, str))
			return (1);
		str++;
	}
	return (0);
}

/**
 * @brief Handle wildcard matches and update new_args.
 *
 * @param char *arg The argument to check for wildcards
 * @param char **files List of files to search in
 * @param char ***new_args The list of new arguments to update
 */
void	handle_wildcard_match(char *arg, char **files, char ***new_args)
{
	char	**match;

	if (ft_strchr(arg, '*'))
	{
		match = expand_wildcard(&arg, files);
		if (match)
		{
			if (!(*new_args))
				*new_args = ft_tabdup((const char **)match);
			else
				*new_args = ft_tabjoin(*new_args, match);
			ft_tabfree(match);
		}
	}
	else
		ft_tabadd(new_args, arg);
}
