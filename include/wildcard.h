/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:18:45 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/17 10:18:45 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_H
# define WILDCARD_H

# include "minishell.h"

// ########################################################
// #						WILDCARD					  #
// ########################################################

int	match_wildcard(const char *pattern, const char *str);
char	**expand_wildcard(char **str, char **files);
void	expand_wildcards(char ***args);

// ########################################################
// #					WILDCARD_UTILS					  #
// ########################################################

void	load_files_if_null(char ***files);
int		handle_wildcard(const char *pattern, const char *str);
void	handle_wildcard_match(char *arg, char **files, char ***new_args);

#endif
