/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

// ########################################################
// #						ENV_VARIABLE				  #
// ########################################################

char			*get_path(char *cmd, char **envp);
char			*get_var_value(char **env, char *var);
const char		*get_var_value_const(char **env, char *var);
char			*replace_variables(char **env, char *str);

#endif
