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

/**
 * @brief print autocomplete possibilities + prompt
 * @param minishell
 * @param dict
 * @param input
 */
void	tab_print(t_minishell *minishell, t_dict *dict, char **input)
{
	minishell->completion->tab_count++;
	ft_putstr_fd("\nFound\n", 1);
	stress_print(dict);
	ft_putstr_fd("\n", 1);
	print_terminal_prompt(minishell, ft_strlen(*input) <= 0);
	ft_putstr_fd(*input, 1);
}

/**
 * @brief Take last word of input and search in the BST
 * 			- if found, cut the BST, if not, print error
 * 			- if too many possibilities, ask user if he wants to see all
 * @param minishell
 * @param input
 * @return 1 if error, 0 if success
 */
int	tab_completion(t_minishell *minishell, char	**input)
{
	char	**search;
	t_dict	*tmp;
	int		count_word;

	(void) minishell;
	(void) tmp;

	if (ft_strlen(*input) <= 0)
		return (1);
	minishell->completion->tab_count++;
	search = ft_split(*input, (const char *) " ");
	count_word = ft_count_words(*input, (const char *) " ") - 1;
	minishell->tab_dict = bst_copy(minishell->dict);
	minishell->tab_dict = search_node(minishell->tab_dict, search[count_word]);
	minishell->tab_dict = cut_node(minishell->tab_dict, search[count_word]);
	if (!minishell->tab_dict)
	{
		ft_putstr_fd("\nNo match found\n", 1);
		print_terminal_prompt(minishell, ft_strlen(*input) <= 0);
		ft_putstr_fd(*input, 1);
		return (1);
	}
	else if (minishell->completion->tab_count == 1
		&& bst_size(minishell->tab_dict) > 20)
	{
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("do you wish to see all ", 1);
		ft_putnbr_fd(bst_size(minishell->tab_dict), 1);
		ft_putstr_fd(" possibilities ? [y/n]\n", 1);
		print_terminal_prompt(minishell, ft_strlen(*input) <= 0);
		return (1);
	}
	else
		tab_print(minishell, minishell->tab_dict, input);
	return (0);
}
