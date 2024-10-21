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

int	str_is_alnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '=')
			return (0);
		i++;
	}
	return (1);
}

static int	check_inside_args(t_cmd *cmd)
{
	char	**cut_name;
	char	*tmp;
	int		i;

	i = 0;
	while (cmd->args[i])
	{
		cut_name = ft_split_quote(cmd->args[i], "=", "\"\'");
		tmp = ft_strtrim(cut_name[0], "=");
		if (!str_is_alnum(tmp))
		{
			ft_tabfree(cut_name);
			ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n",
				tmp);
			free(tmp);
			return (1);
		}
		free(tmp);
		ft_tabfree(cut_name);
		i++;
	}
	return (0);
}

int	export_wt_args(t_minishell *minishell, t_cmd *cmd)
{
	int		position;
	char	*tmp;

	tmp = ft_strjoin(cmd->args[1], "=");
	position = find_table_args(cmd->env, tmp);
	if (position == -1)
		position = find_table_args(minishell->env, cmd->args[1]);
	free(tmp);
	check_inside_args(cmd);
	if (position == -1)
	{
		minishell->env = ft_tabinsert(minishell->env, cmd->args[1],
				ft_tablen((const char **)minishell->env));
		ft_sort(minishell->env, 0,
			ft_tablen((const char **)minishell->env) - 1);
	}
	else if (ft_is_charset('=', cmd->args[1]))
		modify_cmd_env(cmd, cmd->args[1], "");
	return (0);
}

int	command_export(t_minishell *minishell, t_cmd *cmd)
{
	char	**cut_name;

	if (!cmd->args[1])
		return (print_export(cmd));
	else if (ft_is_charset('+', cmd->args[1]))
		cut_name = ft_split_quote(cmd->args[1], "+", "\"\'");
	else if (ft_is_charset('=', cmd->args[1]))
		cut_name = ft_split_quote(cmd->args[1], "=", "\"\'");
	else
		return (export_wt_args(minishell, cmd));
	if (check_inside_args(cmd))
	{
		ft_tabfree(cut_name);
		return (1);
	}
	if (find_table_args(cmd->env, cut_name[0]) == -1)
		add_cmd_env(minishell, cut_name[0], cut_name[1]);
	else if (ft_is_charset('+', cmd->args[1]))
		sum_cmd_env(cmd, cut_name[0], cut_name[1] + 1);
	else
		modify_cmd_env(cmd, cut_name[0], cut_name[1]);
	ft_tabfree(cut_name);
	return (0);
}
