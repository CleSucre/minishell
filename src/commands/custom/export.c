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

static int	parse_export(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	while (str[i])
	{
		if ((str[i] == '+') && i < len - 1)
		{
			ft_fprintf(2, "la\n", str);
			return (1);
		}
		if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '+')
		{
			ft_fprintf(2, "ici\n", str);
			return (1);
		}
		i++;
	}
	return (0);
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
		if (parse_export(tmp) == 1)
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
