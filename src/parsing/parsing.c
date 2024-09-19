/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:27:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/09/11 19:27:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char **ft_taballoc(size_t size)
{
	char	**res;

	res = ft_calloc(size, sizeof(char *));
	if (!res)
		return (NULL);

	while (size--)
	{
		res[size] = ft_calloc(1, sizeof(char));
		if (!res[size])
		{
			ft_tabfree(res);
			return (NULL);
		}
	}
	return (res);
}

static void	ft_extract_or(t_ast_node *ast, char *str)
{
	char 		**tmp;
	int 		index;

	index = ft_str_locate(str, 0, "||");
	while (index != -1)
	{
		ft_printf("or_current_index: %d\n", index);
		if (
				!ft_is_between(str, index, '"', '"')
				&& !ft_is_between(str, index, '\'', '\'')
				&& !ft_is_between(str, index, '(', ')')
				)
		{
			tmp = ft_split_index(str, index);

		}
		index = ft_str_locate(str, index + 1, "||");
	}
}

void	ft_extract_and(t_ast_node *ast)
{
	char		**splited;
	int			index;
	char 		*str;

	str = ast->value;
	index = ft_str_locate(str, 0, "&&");
	while (index > 0)
	{
		ft_printf("and_current_index: %d\n", index);
		if (
				!ft_is_between(str, index, '"', '"')
				&& !ft_is_between(str, index, '\'', '\'')
				&& !ft_is_between(str, index, '(', ')')
				)
		{
			splited = ft_split_index(str, index);
			if (ft_tablen((const char **)splited) == 2)
			{
				ft_printf("splited[0]: %s\n", splited[0]);
				ft_printf("splited[1]: %s\n", splited[1]);
				ast->left = create_ast(AST_COMMAND, splited[0]);
				ast->right = create_ast(AST_COMMAND, splited[1]);
				ast = ast->right;
			}
			ft_tabfree(splited);
		}
		index = ft_str_locate(str, index + 1, "&&");
	}
}

t_ast_node			*parsing_bonus(char *input)
{
	t_ast_node	*ast;

	ast = create_ast(AST_COMMAND, input);
	ft_extract_and(ast);
	if (!ast)
		return (NULL);
	return (ast);
}

/**
 * @brief Parses the input string and creates an AST.
 *
 * @param char *input Input string to parse.
 * @return t_ast * AST created from the input.
 */
t_ast_node	*parse_input(t_minishell *minishell, char *input)
{
	t_ast_node	*ast;

	(void)minishell;
	if (!input)
		return (NULL);
	ast = parsing_bonus(input);
	if (!ast)
		return (NULL);
	debug_ast(ast);
	return (ast);
}
