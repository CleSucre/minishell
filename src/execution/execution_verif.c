/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_verif.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <pierrot.maxime.pro@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 01:34:35 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/24 01:34:35 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exceed_max_heredoc(t_minishell *minishell, t_ast_node *ast)
{
	ft_putstr_fd("minishell: maximum here-document count exceeded\n",
		STDERR_FILENO);
	free_ast(ast);
	close_all_fds(minishell->opened_fds);
	minishell->ast = NULL;
	minishell->exit_code = 2;
	minishell->exit_signal = 1;
	enable_termios(minishell->term);
}

int	pre_executed_failed(t_minishell *minishell, t_ast_node *ast)
{
	enable_termios(minishell->term);
	free_ast(ast);
	close_all_fds(minishell->opened_fds);
	minishell->ast = NULL;
	return (0);
}

/**
 * @brief Verify if the redirection is valid
 *
 * @param t_minishell *minishell
 * @param t_ast_node *ast
 * @return int 1 on success, 0 on failure
 */
int	verify_redirection(t_minishell *minishell, t_ast_node *ast)
{
	char	*start;
	char	*tmp;
	char	**tab_tmp;

	start = ft_strdup(ast->right->value[0]);
	tmp = replace_variables(minishell, ast->right->value[0]);
	free(ast->right->value[0]);
	ast->right->value[0] = tmp;
	tab_tmp = ft_split(ast->right->value[0], WHITESPACES);
	if (!tab_tmp[0] || !ft_strlen(tab_tmp[0]))
		return (free(start), ft_putstr_fd("Error: no file specified\n",
				STDERR_FILENO), 0);
	expand_wildcards(&tab_tmp);
	if (ft_tablen((const char **)tab_tmp) > 1)
	{
		ft_fprintf(STDERR_FILENO, "minishell: %s: ambiguous redirect\n", start);
		free(start);
		ft_tabfree(tab_tmp);
		minishell->exit_code = 1;
		return (0);
	}
	free(start);
	ft_tabfree(tab_tmp);
	return (1);
}
