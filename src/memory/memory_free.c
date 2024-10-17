/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 03:58:37 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/05 03:58:37 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Free history linked list
 *
 * @param t_history *history
 * @return void
 */
static void	history_free(t_history *history)
{
	t_history	*tmp;

	while (history)
	{
		tmp = history;
		history = history->older;
		free(tmp->cmd);
		free(tmp);
	}
}

//TODO: remove !!!!!!!!
static void	free_dirinfo(t_dirinfo *dirinfo)
{
	free(dirinfo);
}

/**
 * @brief Free minishell structure
 *
 * @param t_minishell *minishell
 * @return void
 */
void	free_minishell(t_minishell *minishell)
{
	ft_tabfree(minishell->cache->input);
	free(minishell->cache);
	free(minishell->term);
	history_free(minishell->history);
	free_branch(minishell->tab_dict);
	free_branch(minishell->dict);
	free(minishell->completion);
	ft_tabfree(minishell->input);
	free_dirinfo(minishell->dirinfo);
	free(minishell->starting_path);
	free_ast(minishell->ast);
	ft_tabfree(minishell->env);
	free(minishell);
}

/**
 * @brief Free ast tree
 *
 * @param t_ast_node *ast
 * @return void
 */
void	free_ast(t_ast_node *ast)
{
	int	i;

	if (ast == NULL)
		return ;
	if (ast->left != NULL)
		free_ast(ast->left);
	if (ast->right != NULL)
		free_ast(ast->right);
	if (ast->value != NULL)
	{
		i = 0;
		while (ast->value[i] != NULL)
		{
			free(ast->value[i]);
			i++;
		}
		free(ast->value);
	}
	free(ast);
}

/**
 * @brief Free tokens linked list
 *
 * @param t_token *tokens
 */
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
