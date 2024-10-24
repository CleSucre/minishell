/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_replacer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 21:00:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/12 21:00:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Replace `$?` with the last exit code in the result string.
 *
 * @param minishell The shell context containing the exit code.
 * @param res The current result string.
 * @return Updated result string with the exit code.
 */
static char	*handle_exit_code(t_minishell *minishell, char *res)
{
	char	*tmp;

	tmp = ft_itoa(minishell->exit_code);
	res = ft_strjoin_free(res, tmp);
	return (res);
}

/**
 * @brief Replace `$VARNAME` with its value in the result string.
 *
 * @param minishell The shell context with environment variables.
 * @param str The input string with the variable.
 * @param i The current index in the input string.
 * @param res The current result string.
 * @return Updated result string with the variable value.
 */
static char	*handle_variable(t_minishell *minishell, char *str,
							size_t *i, char *res)
{
	int		j;
	char	*tmp;

	j = *i + 1;
	while (ft_isalnum(str[j]))
		j++;
	tmp = ft_substr(str, *i + 1, j - *i - 1);
	res = ft_strjoin_free(res, get_var_value(minishell->env, tmp));
	free(tmp);
	*i = j;
	return (res);
}

/**
 * @brief Process quotes and handle variables ($VARNAME or $?).
 *
 * @param t_minishell *minishell
 * @param char *str
 * @param char **res
 * @param t_replace_context *ctx (context structure holding i and quotes status)
 */
void	process_quotes_and_dollar(t_minishell *minishell, char *str,
							char **res, t_replace_context *ctx)
{
	if (str[ctx->i] == '\'' && !(ctx->inside_double_quotes))
		ctx->inside_single_quotes = !(ctx->inside_single_quotes);
	else if (str[ctx->i] == '"' && !(ctx->inside_single_quotes))
		ctx->inside_double_quotes = !(ctx->inside_double_quotes);
	else if (str[ctx->i] == '$' && str[ctx->i + 1] != '\0'
		&& (!(ctx->inside_single_quotes) || ctx->inside_double_quotes))
	{
		if (str[ctx->i + 1] == '?')
		{
			*res = handle_exit_code(minishell, *res);
			ctx->i += 2;
		}
		else
			*res = handle_variable(minishell, str, &(ctx->i), *res);
	}
}

/**
 * @brief Replace the variables in a string by their values
 * 			using the env variables (only for the $VARNAME format)
 *
 * @param t_minishell *minishell
 * @param char *str
 * @return char* string with replaced variables
 */
char	*replace_variables(t_minishell *minishell, char *str)
{
	char				*res;
	t_replace_context	ctx;

	ctx.inside_single_quotes = 0;
	ctx.inside_double_quotes = 0;
	ctx.i = 0;
	res = NULL;
	while (str[ctx.i])
	{
		process_quotes_and_dollar(minishell, str, &res, &ctx);
		if (str[ctx.i] && (str[ctx.i] != '$' || ctx.inside_single_quotes))
			res = ft_strjoin_char(res, str[ctx.i++]);
	}
	if (res)
		return (res);
	return (ft_strdup(""));
}

/**
 * @brief Replace the variables in a tab of strings by their values
 *
 * @param t_minishell *minishell
 * @param char **strs
 */
void	replace_variables_in_tab(t_minishell *minishell, char **strs)
{
	char	*tmp;
	int		i;

	i = 0;
	while (strs[i])
	{
		tmp = replace_variables(minishell, strs[i]);
		free(strs[i]);
		strs[i] = tmp;
		i++;
	}
	i = 0;
	while (strs[i])
	{
		if (strs[i][0] == '\0')
		{
			ft_tabdel(strs, i);
			i--;
		}
		i++;
	}
}
