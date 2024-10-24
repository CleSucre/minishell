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

int	export_wt_args(t_minishell *minishell, t_cmd *cmd, int i)
{
	int		position;
	char	*tmp;

	tmp = ft_strjoin(cmd->args[1], "=");
	position = find_table_args(cmd->env, tmp);
	if (position == -1)
		position = find_table_args(minishell->env, cmd->args[i]);
	free(tmp);
	check_inside_args(cmd);
	if (position == -1)
	{
		minishell->env = ft_tabinsert(minishell->env, cmd->args[i],
				ft_tablen((const char **)minishell->env));
		ft_sort(minishell->env, 0,
			ft_tablen((const char **)minishell->env) - 1);
	}
	else if (ft_is_charset('=', cmd->args[1]))
		modify_cmd_env(cmd, cmd->args[i], "");
	return (0);
}

void	update_environment(t_minishell *minishell, t_cmd *cmd,
							char **cut_name, int i)
{
	if (find_table_args(minishell->env, cut_name[0]) == -1)
	{
		add_cmd_env(minishell, cut_name[0], cut_name[1]);
		cmd->env = minishell->env;
	}
	else if (ft_is_charset('+', cmd->args[i]))
		sum_cmd_env(cmd, cut_name[0], cut_name[1] + 1);
	else
		modify_cmd_env(cmd, cut_name[0], cut_name[1]);
}

void	process_export_argument(t_minishell *minishell, t_cmd *cmd, int i)
{
	char	**cut_name;

	cut_name = NULL;
	if (ft_is_charset('+', cmd->args[i]))
		cut_name = ft_split_quote(cmd->args[i], "+", "\"\'");
	else if (ft_is_charset('=', cmd->args[i]))
		cut_name = ft_split_quote(cmd->args[i], "=", "\"\'");
	else
	{
		export_wt_args(minishell, cmd, i);
		cmd->env = minishell->env;
		return ;
	}
	update_environment(minishell, cmd, cut_name, i);
	ft_tabfree(cut_name);
	cut_name = NULL;
}

/**
 * @brief Export the variable in the environment given in parameter
 *
 * @param t_cmd *cmd Command structure
 * @return int Exit code
 */
int	command_export(t_minishell *minishell, t_cmd *cmd)
{
	int	i;

	if (!cmd->args[1])
		return (print_export(cmd));
	i = 1;
	while (cmd->args[i])
	{
		if (check_inside_args(cmd))
			return (1);
		process_export_argument(minishell, cmd, i);
		i++;
	}
	return (0);
}
