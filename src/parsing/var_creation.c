/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 01:00:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/09/21 21:02:57 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
*
*	ARG="string"
*	ARG+="string" -> ARG=string1string2
*
*	Take arg : ARGS + a string, replace args by string inside the input
*	@return args replaced
*
*	Split quote ?
*/

/**
* @brief function that take input, check presence in env
		if exit, clear old value, modify input env by new value;
		else return -1
	@return 0 if ok, 1 if doesn't exist in env
*/
	int	modify_env_input(t_minishell *minishell, char *input, char *value)
	{
		int		is_here;
        char	*res;

		is_here = 0;
		is_here = find_table_args(minishell->env, input);
		if (is_here == -1)
			return (-1);
//		ft_printf("vAVANT la env : [%s]\n", minishell->env[is_here]);
		res = ft_strjoin(ft_strjoin(input, "="), value);
        res = ft_strjoin(res, value);
		clear_string(minishell->env[is_here]);
		ft_strlcpy(minishell->env[is_here], res, ft_strlen(res)+1);
//        ft_printf("vla env : [%s]\n", minishell->env[is_here]);
		return (0);
	}

/*
	int	add_value_arg(t_minishell *minishell, char *input, char *value)
	{
		//Old value, new value, concatenate both of them using modify env input
		int is_here;

		is_here = find_table_args(minishell->env, input);
		if (is_here == -1)
			ft_printf("Create a new env variable\n");


		return (0);
	}
*/

	void	get_input_converter(t_minishell *minishell, t_cmd *input)
	{
		char	**cut_name;
		char	quote_set[3];

		quote_set[0] = 34;
		quote_set[1] = 39;
		quote_set[2] = '\0';

		cut_name = ft_split_quote(input->name, "=", quote_set);
		if (!cut_name)
		{
			ft_fprintf(2, "Error in parsing input converter\n");
			return ;
		}
		if (ft_is_charset('+', cut_name[0]))
		{
			ft_fprintf(2, "join env to value\n");
			return ;
		}
//		int i = 0;
//		while (cut_name[i])
//		{
//			ft_printf("cut_name [%s]\n", cut_name[i]);
//			i++;
//		}
		if (modify_env_input(minishell, cut_name[0], cut_name[1]) == -1)
		{
			clear_string(input->name);
			ft_strlcpy(input->name, cut_name[1], ft_strlen(cut_name[1])+1);
		}
	}
