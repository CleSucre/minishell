/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stress_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/09/10 22:31:09 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Print the branch found
 *
 * @param t_dict *dict Dict structure
 */
void	print_branch(t_dict *dict)
{
	if (!dict)
	{
		ft_putstr_fd("NULL\n", 1);
		return ;
	}
	ft_putstr_fd(" key:   ", 1);
	ft_putstr_fd(dict->key, 1);
	ft_putstr_fd("\n", 1);
	if (dict->left_branch)
		print_branch(dict->left_branch);
	if (dict->right_branch)
		print_branch(dict->right_branch);
}

/**
 * @brief Print the branch found but
 *
 * @param t_minishell *minishell
 * @param t_dict *dict
 */
static void	stress_print_with_nl(t_minishell *minishell, t_dict *dict)
{
	ft_putstr_fd("\n", 1);
	minishell->completion->print_line++;
	if (dict->position == minishell->completion->tab_count)
		ft_putstr_fd("\033[7m", 1);
	write(1, dict->key, ft_strlen(dict->key));
	if (dict->position == minishell->completion->tab_count)
		ft_putstr_fd("\033[0m", 1);
	repeat_str(" ", (20 - ft_strlen(dict->key)));
}

/**
 * @brief Print the branch found with stress condition
 *
 * @param t_minishell *minishell
 * @param t_dict *dict
 */
void	stress_print(t_minishell *minishell, t_dict *dict)
{
	t_term	*term;

	if (!dict)
		return ;
	term = ft_calloc(1, sizeof(t_term));
	get_cursor_position(term);
	get_terminal_size(term);
	if (((ft_strlen(dict->key) + term->cols)
			+ term->ws_cols / 5) < term->ws_cols)
	{
		if (dict->position == minishell->completion->tab_count)
			ft_putstr_fd("\033[7m", 1);
		ft_putstr_fd(dict->key, 1);
		if (dict->position == minishell->completion->tab_count)
			ft_putstr_fd("\033[0m", 1);
		repeat_str(" ", (20 - ft_strlen(dict->key)));
	}
	else
		stress_print_with_nl(minishell, dict);
	if (dict->left_branch)
		stress_print(minishell, dict->left_branch);
	if (dict->right_branch)
		stress_print(minishell, dict->right_branch);
	free(term);
	term = NULL;
}

/**
 * @brief Create the temporary dictionnary which would be used
 * 				as long as we press tab
 *
 * @param t_minishell *minishell
 * @param char *str
 * @return int 0 if success, 1 if fail
 */
int	creation_tab_dict(t_minishell *minishell, char *str)
{
	int		count_word;
	char	**search;
	t_dict	*head;

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
	return (0);
}
