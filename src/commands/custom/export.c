/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 06:52:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/19 09:45:03 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Export the variable in the environment given in parameter
 *
 * @param t_cmd *cmd Command structure
 * @return int Exit code
 */

char *surround_str_by(char *str, char *surround)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(surround, str);
	res = ft_strjoin(tmp, surround);
	free(tmp);
	return (res);
}

void	print_export(t_cmd *cmd)
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
			ft_fprintf(cmd->output_fd, "=\"%s\"",cut_name[1]);
		else
			ft_fprintf(cmd->output_fd, "=\"\"");
		ft_putstr_fd("\n", cmd->output_fd);
		ft_tabfree(cut_name);
		i++;
	}
	ft_tabfree(tmp);
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
	res = ft_strjoin(tmp, value);
	clear_string(cmd->env[is_here]);
	ft_strncpy(cmd->env[is_here], res, ft_strlen(res) + 1);
	free(tmp);
	free(res);
	return (0);
}

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

int	command_export(t_minishell *minishell, t_cmd *cmd)
{
	char	**cut_name;
	char	*arg_equal;

	cut_name = NULL;
	if (!cmd->args[1])
	{
		print_export(cmd);
		return (0);
	}
	else if (ft_is_charset('+', cmd->args[1]))
		cut_name = ft_split_quote(cmd->args[1], "+", "\"\'");
	else if (ft_is_charset('=', cmd->args[1]))
		cut_name = ft_split_quote(cmd->args[1], "=", "\"\'");
	arg_equal = ft_strjoin(cut_name[0], "=");
	if (find_table_args(cmd->env, arg_equal) == -1)
		add_cmd_env(minishell, cut_name[0], cut_name[1]);
	else
	{
		if (ft_is_charset('+', cmd->args[1]))
			sum_cmd_env(cmd, cut_name[0], cut_name[1] + 1);
		else
			modify_cmd_env(cmd, cut_name[0], cut_name[1]);
	}
	free(arg_equal);
	ft_tabfree(cut_name);
	return (0);
}
