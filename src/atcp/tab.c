/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 04:21:45 by mpierrot          #+#    #+#             */
/*   Updated: 2024/08/12 04:06:45 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prompt_completion(t_minishell *minishell, char **input)
{
	bst_size(minishell->tab_dict);
	ft_putstr_fd("\n", 1);
	print_terminal_prompt(minishell, ft_strlen(*input) <= 0);
	minishell->tab_dict = move_in_bst(minishell->tab_dict,
			minishell->completion->tab_count);
	if (ft_strcmp(*input, minishell->tab_dict->key) != 0)
	{
		ft_trunc(input, 1);
		*input = ft_strjoin(*input, minishell->tab_dict->key);
	}
	ft_putstr_fd(*input, 1);
	get_cursor_position(minishell->term);
	minishell->completion->tab_count = 0;
	minishell->completion->print_line = 1;
	minishell->completion->check_len = 0;
	free_branch(minishell->tab_dict);
	minishell->tab_dict = NULL;
}

/**
 * @brief print autocomplete possibilities + prompt
 * @param minishell
 * @param dict
 * @param input
 */
void	tab_print(t_minishell *minishell, t_dict *dict, char **input)
{
	minishell->completion->tab_count++;
	if (minishell->completion->tab_count > (int)bst_size(dict))
		minishell->completion->tab_count = 1;
	ft_putstr_fd("\033[s", 1);
	ft_putstr_fd("\n", 1);
	minishell->completion->print_line++;
	stress_print(minishell, dict);
	ft_putstr_fd("\n", 1);
	print_terminal_prompt(minishell, ft_strlen(*input) <= 0);
	ft_putstr_fd(*input, 1);
	minishell->completion->check_len = 0;
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
	char	*str;
	char	**search;
	int		count_word;
	t_dict	*head;

	ft_tabdel(input, ft_tablen((const char **)input) - 1);
	str = ft_utf8_tab_to_str(input);
	if (ft_strlen(str) <= 0)
		return (1);
	if (minishell->tab_dict)
		free_branch(minishell->tab_dict);
	search = ft_split(str, (const char *) " ");
	if (!search)
		return (1);
	count_word = ft_count_words(str, (const char *) " ") - 1;
	minishell->tab_dict = bst_copy(minishell->dict);
	head = search_node(minishell->tab_dict, search[count_word]);
	free_branch(minishell->tab_dict);
	minishell->tab_dict = NULL;
	cut_node(head, search[count_word]);
	minishell->tab_dict = bst_copy(head);
	free_branch(head);
	head = NULL;
	ft_tabfree(search);
	if (!minishell->tab_dict)
	{
		ft_putstr_fd("\nNo match found\n", 1);
		print_terminal_prompt(minishell, ft_strlen(str) <= 0);
		ft_putstr_fd(str, 1);
		free_branch(head);
		free(str);
		str = NULL;
		head = NULL;
		return (1);
	}
	if (tab_action(minishell, &str))
	{
		free(str);
		str = NULL;
		return (1);
	}
	free(str);
	str = NULL;
	return (0);
}

/**
 * @brief Print the autocomplete possibilities
 * @param minishell
 * @param input
 * @return
 */
int	tab_action(t_minishell *minishell, char **input)
{
	if (minishell->completion->tab_count == 0
		&& bst_size(minishell->tab_dict) > 20
		&& minishell->completion->check_len == 0)
	{
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("do you wish to see all ", 1);
		ft_putnbr_fd(bst_size(minishell->tab_dict), 1);
		ft_putstr_fd(" possibilities ? [y/n]\n", 1);
		print_terminal_prompt(minishell, ft_strlen(*input) <= 0);
		ft_putstr_fd(*input, 1);
		minishell->completion->check_len = 1;
		return (1);
	}
	else
	{
		if (minishell->completion->tab_count > 0)
		{
			while (minishell->completion->print_line > 1)
			{
				ft_putstr_fd("\033[F", 1);
				ft_putstr_fd("\033[2K", 1);
				minishell->completion->print_line--;
			}
			ft_putstr_fd("\033[F", 1);
		}
		tab_print(minishell, minishell->tab_dict, input);
	}
	minishell->completion->check_len = 1;
	return (0);
}

/**
 * @brief Manage tab key by sending it to the right function
 * @param minishell
 * @param input
 * @param c
 * @return
 */
int	tab_manager(t_minishell *minishell, char *new)
{
	if (new[0] == '\t' && !minishell->input)
		return (1);
	if (new[0] == 'n')
	{
		minishell->completion->tab_count = 0;
		minishell->completion->check_len = 0;
		minishell->completion->print_line = 1;
		free_branch(minishell->tab_dict);
		ft_putstr_fd(*minishell->input, 1);
		return (1);
	}
	else
		minishell->input = ft_tabjoin(minishell->input,
				ft_utf8_split_chars(new));
	if (minishell->completion->tab_count == 0 && new[0] == 'y'
		&& minishell->completion->check_len == 0)
		tab_print(minishell, minishell->tab_dict, minishell->input);
	else if (minishell->completion->tab_count == 0)
		tab_completion(minishell, minishell->input);
	else
		tab_action(minishell, minishell->input);
	return (0);
}
