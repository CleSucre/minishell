/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_add.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/19 18:13:42 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sum_cmd_env(t_cmd *cmd, char *input, char *value)
{
	int		is_here;
	char	**arg_split;
	char	*res;
	char	*tmp;

	is_here = 0;
	is_here = find_table_args(cmd->env, input);
	if (is_here == -1)
		return (-1);
	arg_split = ft_split_quote(cmd->env[is_here], "=", "\"\'");
	if (!arg_split)
		return (-1);
	res = ft_strjoin(input, "=");
	tmp = ft_strjoin(res, arg_split[1]);
	free(res);
	res = ft_strjoin(tmp, value);
	free(cmd->env[is_here]);
	cmd->env[is_here] = ft_strdup(res);
	free(res);
	free(tmp);
	ft_tabfree(arg_split);
	return (0);
}

int	add_cmd_env(t_minishell *minishell, char *input, char *value)
{
	char	**tmp;
	char	*res;
	char	*res2;

	tmp = ft_tabdup((const char **)minishell->env);
	res = ft_strjoin(input, "=");
	res2 = ft_strjoin(res, value);
	free(res);
	ft_tabfree(minishell->env);
	minishell->env = ft_tabinsert(tmp, res2, ft_tablen((const char **)tmp));
	free(res2);
	ft_sort(minishell->env, 0, ft_tablen((const char **)minishell->env) - 1);
	return (0);
}
