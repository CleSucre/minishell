/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 04:21:45 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/09 08:43:36 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int tab_completion(t_minishell *minishell, char **input)
{
	(void)minishell;
	(void)input;
//	minishell->completion->tab_count++;
//	ft_putstr_fd("AHAHAHHAHAHHAUdghweuisghfuiowebf", 2);
//	search_in_path(minishell->dict);
	creation_dict(minishell);
	print_terminal_prompt(minishell, ft_strlen(*input) <= 0);

	return (0);
}