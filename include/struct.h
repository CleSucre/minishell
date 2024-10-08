/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:20:34 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/17 02:52:20 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

#include "commands.h"

// ########################################################
// #						TERMINAL					  #
// ########################################################

typedef struct s_term
{
	char			*iterm_buffer;
	char			*strterm_buffer;
	unsigned int	ws_cols;
	unsigned int	ws_rows;
	unsigned int	cols;
	unsigned int	rows;
	unsigned int	begin_rows;
	struct termios	*original_termios;
}		t_term;

// ########################################################
// #						HISTORY						  #
// ########################################################

typedef struct s_history
{
	char				*cmd;
	struct s_history	*older;
	struct s_history	*newer;
}		t_history;

// ########################################################
// #				DICTIONARY	- BST					  #
// ########################################################

typedef struct s_dict
{
	int				bst_size;
	int				position;
	char			*key;
	char			*value;
	struct s_dict	*left_branch;
	struct s_dict	*right_branch;
	struct s_dict	*parent;
}					t_dict;


// ########################################################
// #					AUTOCOMPLETION					  #
// ########################################################

typedef struct s_completion
{
	char	*cmd;
	char	*path;
	int		check_len;
	int		tab_count;
	int		print_line;
}			t_completion;

// ########################################################
// #						MINISHELL					  #
// ########################################################

typedef struct s_cache
{
	char			**input;
}		t_cache;

typedef struct s_minishell
{
	t_cache			*cache;
	t_history		*history;
	t_term			*term;
	t_completion	*completion;
	t_dict			*dict;
	t_dict			*tab_dict;
    char            **input;
	char			**env;
	int				exit_code;
	unsigned int	history_pos;
	unsigned int	history_size;
	struct s_cmd	*current_cmd;
}		t_minishell;

#endif
