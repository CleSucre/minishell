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
	ft_fprintf(STDERR_FILENO, "Error: pre_execute_ast failed\n");
	return (0);
}
