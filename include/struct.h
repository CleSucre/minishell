/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:20:34 by julthoma          #+#    #+#             */
/*   Updated: 2024/10/17 10:21:51 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "commands.h"

// ########################################################
// #						TERMINAL						#
// ########################################################

typedef struct s_term
{
	char				*iterm_buffer;
	char				*strterm_buffer;
	unsigned int		ws_cols;
	unsigned int		ws_rows;
	unsigned int		cols;
	unsigned int		rows;
	unsigned int		begin_rows;
	struct termios		*original_termios;
}						t_term;

// ########################################################
// #						HISTORY							#
// ########################################################

typedef struct s_history
{
	char				*cmd;
	struct s_history	*older;
	struct s_history	*newer;
}						t_history;

// ########################################################
// #				DICTIONARY	- BST						#
// ########################################################

typedef struct s_dict
{
	int					bst_size;
	int					position;
	char				*key;
	char				*value;
	struct s_dict		*left_branch;
	struct s_dict		*right_branch;
	struct s_dict		*parent;
}						t_dict;

// ########################################################
// #					AUTOCOMPLETION						#
// ########################################################

typedef struct s_completion
{
	int					check_len;
	int					tab_count;
	int					print_line;
}						t_completion;

// ########################################################
// #						CD_PATHS						#
// ########################################################

typedef struct s_dirinfo
{
	char				*path;
	char				*old_path;

}						t_dirinfo;
// ########################################################
// #						MINISHELL						#
// ########################################################

typedef struct s_cache
{
	char				**input;
}						t_cache;

typedef struct s_minishell
{
	t_cache				*cache;
	t_history			*history;
	t_term				*term;
	t_completion		*completion;
	t_dict				*dict;
	t_dict				*tab_dict;
	t_dirinfo			*dirinfo;
	struct s_ast_node	*ast;
	char				**input;
	char				**env;
	char				*starting_path;
	int					exit_code;
	int					exit_signal;
	unsigned int		history_pos;
	unsigned int		history_size;
}						t_minishell;

#endif
