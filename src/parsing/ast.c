/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Create ast object
 *
 * @param t_type type of ast
 * @param char *value value of ast
 * @return t_ast *
 */
t_ast	*create_ast(t_type type, char *value)
{
	t_ast	*ast;

	ast = malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->type = type;
	ast->value = ft_strdup(value);
	ast->left = NULL;
	ast->right = NULL;
	return (ast);
}

/**
 * @brief Print the ast
 *
 * @param t_ast *ast
 * @return None
 */
void	print_ast(t_ast *ast)
{
	(void)ast;
}