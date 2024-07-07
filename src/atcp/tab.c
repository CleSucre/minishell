/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 04:21:45 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/07 07:38:28 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int tab_completion(t_minishell *minishell, char **input)
{
	(void)minishell;
	(void)input;
//	minishell->completion->tab_count++;
	ft_putstr_fd("AHAHAHHAHAHHAUdghweuisghfuiowebf", 2);
	search_in_path(minishell->dict);
	return (0);
}