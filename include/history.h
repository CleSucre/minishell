/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# define HISTORY_FILE ".ministory"

// ########################################################
// #					HISTORY_FILE					  #
// ########################################################

int			history_get_file(void);
int			history_load(t_minishell *minishell);

// ########################################################
// #					HISTORY_NAVIGATION				  #
// ########################################################

t_history	*history_find_up(t_minishell *minishell, char *cmd);
t_history	*history_find_down(t_minishell *minishell, char *cmd);

// ########################################################
// #					HISTORY_MANAGEMENT				  #
// ########################################################

int			history_add(t_minishell *minishell, char *cmd, int fs);
void		history_reset(void);
void		history_print(t_minishell *minishell);

#endif
