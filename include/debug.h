/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# ifndef DEBUG
#  define DEBUG 0
# endif

# include "parsing.h"

// ########################################################
// #					DEBUG_EXECUTION						#
// ########################################################

void	debug_execution(char *cmd);

// ########################################################
// #					DEBUG_HISTORY						#
// ########################################################

void	debug_history_loaded(int cmd_count);
void	debug_history_add(char *cmd, int added);
void	debug_history_add_file(char *cmd);

// ########################################################
// #					DEBUG_PARSING						#
// ########################################################

void	debug_tokens(t_token *tokens);
void	debug_children(t_ast_node *ast, int level);
void	debug_ast(t_ast_node *ast);

// ########################################################
// #					DEBUG_PARSING_UTILS				  #
// ########################################################

void	debug_ast_node(t_ast_node *ast, int level);
char	*get_ast_node_type_primary(t_ast_node_type type);
char	*get_ast_node_type_secondary(t_ast_node_type type);
char	*get_token_type(t_token_type type);

#endif
