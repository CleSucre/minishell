/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_prompt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 06:31:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/14 06:31:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Get the prompt length
 *
 * @param t_minishell *minishell
 * @return unsigned int length of the prompt
 */
unsigned int	get_prompt_len(t_minishell *minishell)
{
	const char		*user;
	const char		*host;
	const char		*path;
	const char		*home;
	unsigned int	length;

	user = get_var_value_const(minishell->env, "USER");
	host = get_var_value_const(minishell->env, "NAME");
	path = get_var_value_const(minishell->env, "PWD");
	home = get_var_value_const(minishell->env, "HOME");
	length = ft_strlen(user) + ft_strlen(host);
	if (ft_strncmp(path, home, ft_strlen(home)) == 0)
		length += 1 + ft_strlen(path + ft_strlen(home));
	else
		length += ft_strlen(path);
	length += 4;
	return (length);
}

/**
 * @brief Print the prompt
 *
 * @param t_minishell *minishell
 * @param int new_line 1 if a new line is needed
 * 				before the prompt 0 if not
 * @return void
 */
void	print_terminal_prompt(t_minishell *minishell, int new_line)
{
	const char	*user;
	const char	*host;
	const char	*path;
	const char	*home;

	user = get_var_value_const(minishell->env, "USER");
	host = get_var_value_const(minishell->env, "NAME");
	path = get_var_value_const(minishell->env, "PWD");
	home = get_var_value_const(minishell->env, "HOME");
	if (new_line)
		ft_putchar_fd('\n', 1);
	ft_putstr_fd(GREEN, 1);
	ft_putstr_fd((char *)user, 1);
	ft_putchar_fd('@', 1);
	ft_putstr_fd((char *)host, 1);
	ft_putstr_fd(RESET":", 1);
	ft_putstr_fd(BLUE, 1);
	if (ft_strncmp(path, home, ft_strlen(home)) == 0)
	{
		ft_putstr_fd("~", 1);
		ft_putstr_fd((char *)(path + ft_strlen(home)), 1);
	}
	else
		ft_putstr_fd((char *)path, 1);
	ft_putstr_fd(WHITE "$ " RESET, 1);
}
