/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/10 09:14:14 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get length of ast
 *
 * @param t_ast *ast
 * @return unsigned int
 */
unsigned int	ast_len(t_ast_node *ast)
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
 * @brief Count number of ast with given type
 *
 * @param t_ast *ast
 * @param t_type type
 * @return unsigned int
 */
unsigned int	ast_count_type(t_ast_node *ast, t_ast_node_type type)
{
	unsigned int	count;

	count = 0;
	while (ast)
	{
		if (ast->type == type)
			count++;
		ast = ast->next;
	}
	return (count);
}

/**
 * @brief Create ast object
 *
 * @param t_type type of ast
 * @param char *value value of ast
 * @return t_ast *
 */
t_ast_node	*create_ast(t_ast_node_type type, char *value)
{
	t_ast_node	*ast;

	ast = malloc(sizeof(t_ast_node));
	if (!ast)
		return (NULL);

	ast->type = type;
	ast->value = ft_strdup(value);
	ast->left = NULL;
	ast->right = NULL;
	ast->next = NULL;
	ast->args = NULL;
	ast->redirs = NULL;
	return (ast);
}

/**
 * @brief Get last ast of the list
 *
 * @param t_ast *head
 * @return t_ast *
 */
t_ast_node	*ast_get_last(t_ast_node *head)
{
	t_ast_node	*tmp;

	tmp = head;
	while (tmp->right)
		tmp = tmp->right;
	return (tmp);
}

/**
 * @brief Add ast to the end of the list
 *
 * @param t_ast *head head of the list
 * @param t_ast *ast ast to add
 * @return void
 */
void	ast_add_last(t_ast_node **head, t_ast_node *ast)
{
	t_ast_node	*tmp;

	if (!*head)
	{
		ft_printf("head is null\n");
		*head = ast;
		return ;
	}
	tmp = ast_get_last(*head);
	tmp->right = ast;
	ast->left = tmp;
}

/**
 * @brief Add ast as a children of given ast
 *
 * @param t_ast *head head of the list
 * @param t_ast *ast ast to add
 * @return void
 */
void	ast_add_children(t_ast_node *ast, t_ast_node *children)
{
	t_ast_node	*tmp;

	tmp = ast;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = children;
}
