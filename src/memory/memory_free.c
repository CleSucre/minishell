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
	free(minishell);
}

/**
 * @brief Free ast linked list
 *
 * @param t_ast *ast
 * @return void
 */
void	free_ast(t_ast_node *ast)
{
	t_ast_node	*tmp;

	while (ast)
	{
		tmp = ast;
		ast = ast->next;
		free_ast(tmp->left);
		free_ast(tmp->right);
		if (!tmp->value)
			free(tmp->value);
		if (tmp->next)
			free_ast(tmp->next);
		free(tmp);
	}
}

void	free_cmd(t_cmd *cmd)
{
	free(cmd->cmd_name);
	ft_tabfree(cmd->argv);
	free(cmd->path);
	free(cmd);
}
