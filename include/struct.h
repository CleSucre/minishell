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
	struct s_history	*prev;
	struct s_history	*next;
}		t_history;

// ########################################################
// #					MINISHELL STRUCT				  #
// ########################################################

typedef struct s_termios
{
	struct termios	*original_termios;
}		t_termios;

typedef struct s_minishell
{
	t_history	*history;
	t_termios	*termios;
}		t_minishell;

#endif
