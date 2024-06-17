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
 * @brief Get length of ast
 *
 * @param t_ast *ast
 * @return unsigned int
 */
unsigned int	ast_len(t_ast *ast)
{
	unsigned int	len;

	len = 0;
	while (ast)
	{
		len++;
		ast = ast->next;
	}
	return (len);
}

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
	ast->value = value;
	ast->children = NULL;
	ast->next = NULL;
	ast->prev = NULL;
	return (ast);
}

/**
 * @brief Get last ast of the list
 *
 * @param t_ast *head
 * @return t_ast *
 */
t_ast	*ast_get_last(t_ast *head)
{
	t_ast	*tmp;

	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

/**
 * @brief Add ast to the end of the list
 *
 * @param t_ast *head head of the list
 * @param t_ast *ast ast to add
 * @return void
 */
void	ast_add_last(t_ast **head, t_ast *ast)
{
	t_ast	*tmp;

	if (!*head)
	{
		*head = ast;
		return ;
	}
	tmp = ast_get_last(*head);
	tmp->next = ast;
	ast->prev = tmp;
}

/**
 * @brief Add ast as a children of given ast
 *
 * @param t_ast *head head of the list
 * @param t_ast *ast ast to add
 * @return void
 */
void	ast_add_children(t_ast *ast, t_ast *children)
{
	t_ast	*tmp;

	tmp = ast;
	while (tmp->children)
		tmp = tmp->children;
	tmp->children = children;
}
