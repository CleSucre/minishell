/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_add.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/22 21:16:50 by mpierrot         ###   ########.fr       */
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

int	print_export(t_cmd *cmd)
{
	int		i;
	char	**tmp;
	char	**cut_name;

	tmp = ft_tabdup((const char **)cmd->env);
	ft_sort(tmp, 0, ft_tablen((const char **)tmp) - 1);
	i = 0;
	while (tmp[i])
	{
		cut_name = ft_split_quote(tmp[i], "=", "\"\'");
		ft_putstr_fd("declare -x ", cmd->output_fd);
		ft_putstr_fd(cut_name[0], cmd->output_fd);
		if (cut_name[1])
			ft_fprintf(cmd->output_fd, "=\"%s\"", cut_name[1]);
		else if (ft_is_charset('=', tmp[i]))
			ft_fprintf(cmd->output_fd, "=\"\"");
		ft_putstr_fd("\n", cmd->output_fd);
		ft_tabfree(cut_name);
		i++;
	}
	ft_tabfree(tmp);
	return (0);
}

int	modify_cmd_env(t_cmd *cmd, char *input, char *value)
{
	int		is_here;
	char	*res;
	char	*tmp;

	is_here = 0;
	is_here = find_table_args(cmd->env, input);
	if (is_here == -1)
		return (-1);
	tmp = ft_strjoin(input, "=");
	if (!tmp)
		return (-1);
	res = ft_strjoin(tmp, value);
	if (!res)
	{
		free(tmp);
		return (-1);
	}
	clear_string(cmd->env[is_here]);
	free(cmd->env[is_here]);
	cmd->env[is_here] = ft_strdup(res);
	free(tmp);
	free(res);
	return (0);
}
