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
// #						AST STRUCT					  #
// ########################################################

// ########################################################
// #					HISTORY STRUCT					  #
// ########################################################

typedef struct s_history
{
	char				*cmd;
	struct s_history	*older;
	struct s_history	*newer;
}		t_history;

// ########################################################
// #					MINISHELL STRUCT				  #
// ########################################################

typedef struct s_termios
{
	struct termios	*original_termios;
}		t_termios;

typedef struct s_cache
{
	char			*input;
}		t_cache;

typedef struct s_minishell
{
	t_cache			*cache;
	t_history		*history;
	t_termios		*termios;
	int				exit_code;
	unsigned int	history_pos;
	unsigned int	history_size;

}		t_minishell;

#endif
