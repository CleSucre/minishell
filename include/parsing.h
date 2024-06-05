/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_PARSING_H
# define MINISHELL_PARSING_H

typedef enum {
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	COMMAND,
	ARGUMENT,
    QUOTE,
    VARIABLE
}	t_type;

typedef struct s_ast
{
	char			*value;
	int				type;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_token
{
	t_type	type;
	char	*value;
	struct s_token	*next;
}	t_token;

// ########################################################
// #						PARSER						  #
// ########################################################

t_ast		*parse_input(t_minishell *minishell, char *input);

// ########################################################
// #						TOKENIZER					  #
// ########################################################

t_token		*create_token(t_type type, char *value);
t_token		*tokenize(char *input);


// ########################################################
// #						AST						  #
// ########################################################

t_ast		*create_ast(t_type type, char *value);

// ########################################################
// #						DEBUG						  #
// ########################################################

void	print_tokens(t_token *tokens);
void	print_ast(t_ast *ast);

#endif
