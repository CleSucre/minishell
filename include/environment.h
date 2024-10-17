/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:22:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/17 10:22:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

// ########################################################
// #						ENV_VARIABLE					#
// ########################################################

char		*get_path(char *cmd, char **envp);
char		*get_var_value(char **env, char *var);
const char	*get_var_value_const(char **env, char *var);
char		*replace_variables(t_minishell *minishell, char *str);
void		replace_variables_in_tab(t_minishell *minishell, char **strs);
int			reload_env(char **env);

#endif
