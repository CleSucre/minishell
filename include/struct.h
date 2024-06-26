/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                        :+:      :+:    :+:   	  */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

// ########################################################
// #						TERMINAL					  #
// ########################################################

typedef struct s_term
{
	char *			iterm_buffer;
	char *			strterm_buffer;
	unsigned int	size;
	unsigned int	cols;
	unsigned int	rows;
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
// #						MINISHELL					  #
// ########################################################

typedef struct s_cache
{
    char            *prompt;
	unsigned int	prompt_len;
	char            *input;
}		t_cache;

typedef struct s_minishell
{
	t_cache			*cache;
	t_history		*history;
	t_term			*term;
	int				exit_code;
	unsigned int	history_pos;
	unsigned int	history_size;
}		t_minishell;

#endif
