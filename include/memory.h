/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:33:26 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/17 10:33:26 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include "parsing.h"

// ##########################################################
// #						ALLOC							#
// ##########################################################

t_minishell	*alloc_minishell(void);
int			alloc_dirinfo(t_minishell *minishell);
int			alloc_starting_path(t_minishell *minishell);

// ##########################################################
// #						FREE							#
// ##########################################################

void		free_minishell(t_minishell *minishell);
void		free_ast(t_ast_node *ast);
void		free_tokens(t_token *tokens);

#endif
