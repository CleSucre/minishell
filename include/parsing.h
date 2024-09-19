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

# include "struct.h"

typedef enum s_token_type
{
	TOKEN_COMMAND,     // Commande (comme "ls", "echo", etc.)
	TOKEN_ARGUMENT,    // Argument d'une commande (comme "-l", "/home/user")
	TOKEN_OPERATOR,    // Opérateurs logiques (&&, ||)
	TOKEN_PIPE,        // Pipe (|)
	TOKEN_REDIRECTION, // Redirection (>, <)
	TOKEN_PARENTHESIS  // Parenthèse gauche ou droite
}   t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum s_ast_node_type
{
	AST_COMMAND,        // A simple command
	AST_PIPE,           // A pipe '|'
	AST_AND,            // Logical AND '&&'
	AST_OR,             // Logical OR '||'
	AST_SEQUENCE,       // Sequence of commands
	AST_SUBSHELL,       // A subshell '(...)'
	AST_REDIRECTION,    // Input/output redirection
	AST_ASSIGNMENT,     // Variable assignment
	AST_VARIABLE,       // Variable usage
}   t_ast_node_type;

typedef struct s_ast_node
{
	t_ast_node_type		type;       // Type of the AST node
	char                *value;     // Command name or literal value
	struct s_ast_node   *left;      // Left next (e.g., command or left operand)
	struct s_ast_node   *right;     // Right next (e.g., argument or right operand)
	struct s_ast_node   *next;      // Next node in a sequence (e.g., next command)
	struct s_ast_node   *args;      // Arguments for commands
	struct s_ast_node   *redirs;    // Redirections for commands
}   t_ast_node;

// ########################################################
// #						PARSER						  #
// ########################################################

t_ast_node			*parse_input(t_minishell *minishell, char *input);

// ########################################################
// #					TOKEN_MANAGER					  #
// ########################################################

t_token				*new_token(char *str, int type);
void				add_token(t_token **head, t_token *new);
void				free_tokens(t_token *head);

// ########################################################
// #						TOKENIZER					  #
// ########################################################

t_token_type		token_type(char *str);
t_token				*tokenize(char *input);

// ########################################################
// #					PARSER_MANDATORY				  #
// ########################################################

t_ast_node			*extract_full_commands(char *input);
void			*parse_full_commands(t_ast_node *ast);

// ########################################################
// #					PARSER_BONUS					  #
// ########################################################

t_ast_node			*parsing_bonus(char *input);

// ########################################################
// #						TOKENIZER					  #
// ########################################################

t_ast_node_type			token_type_primary(char *str);

// ########################################################
// #					AST_CREATION					  #
// ########################################################

void			parse_args(t_ast_node *ast, char **args);

// ########################################################
// #						AST						  	  #
// ########################################################

unsigned int	ast_len(t_ast_node *ast);
unsigned int	ast_count_type(t_ast_node *ast, t_ast_node_type type);
t_ast_node			*create_ast(t_ast_node_type type, char *value);
t_ast_node			*ast_get_last(t_ast_node *head);
void			ast_add_last(t_ast_node **head, t_ast_node *ast);
void			ast_add_children(t_ast_node *ast, t_ast_node *children);

// ########################################################
// #						DEBUG						  #
// ########################################################

void			print_tokens(t_token *tokens);
void			print_ast(t_ast_node *ast);

#endif
