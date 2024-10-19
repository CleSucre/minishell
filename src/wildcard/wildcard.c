/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:09:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/17 10:09:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the string matches the wildcard
 *
 * @param const char *pattern The wildcard pattern
 * @param const char *str The string to match
 * @return int 1 if the string matches the wildcard, 0 otherwise
 */
int	match_wildcard(const char *pattern, const char *str)
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
			return (handle_wildcard(pattern, str));
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

/**
 * @brief Count the number of files matching the wildcard
 *
 * @param char *search The wildcard to search
 * @param char **files The list of files to search in
 * @return int The number of files matching the wildcard
 */
static int	count_matches(char *search, char **files)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (files[i])
	{
		if (match_wildcard(search, files[i]))
			count++;
		i++;
	}
	return (count);
}

/**
 * @brief Expand the wildcard in the string
 *
 * @param char **str The string to expand
 * @param char **files The list of files to search in
 * @return char** The list of files matching the wildcard
 */
char	**expand_wildcard(char **str, char **files)
{
	char	**match;
	int		i;
	int		j;
	int		match_count;
	char	*search;

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
			match[j] = ft_strdup(files[i]);
			j++;
		}
		i++;
	}
	match[j] = NULL;
	return (match);
}

/**
 * @brief Support wildcard expansion.
 *        Replace the wildcard with the list of
 *        files matching it only if there is a match.
 *        Search the files in the current directory.
 *
 * @param char ***args The arguments to expand
 */
void	expand_wildcards(char ***args)
{
	int		i;
	char	**files;
	char	**new_args;
	char	**current_args;

	files = NULL;
	new_args = NULL;
	i = 0;
	current_args = *args;
	while (current_args[i])
	{
		load_files_if_null(&files);
		handle_wildcard_match(current_args[i], files, &new_args);
		i++;
	}
	if (files)
		ft_tabfree(files);
	if (new_args)
	{
		ft_tabfree(*args);
		*args = new_args;
	}
}
