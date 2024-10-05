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

#define MAX_COMMAND_SIZE 1024

typedef enum s_token_type
{
	TOKEN_COMMAND,              // Command (e.g., "ls", "echo", etc.)
	TOKEN_ARGUMENT,             // Argument of a command (e.g., "-l", "/home/user")
	TOKEN_AND_OPERATOR,         // Logical operator (&&)
	TOKEN_OR_OPERATOR,          // Logical operator (||)
	TOKEN_PIPE,                 // Pipe (|)
	TOKEN_REDIR_OUT,            // Output redirection (>)
	TOKEN_REDIR_OUT_APPEND,     // Output redirection in append mode (>>)
	TOKEN_REDIR_IN,             // Input redirection (<)
	TOKEN_HEREDOC,              // Heredoc redirection (<<)
	TOKEN_PARENTHESIS_OPEN,     // Opening parenthesis
	TOKEN_PARENTHESIS_CLOSE,    // Closing parenthesis
	TOKEN_VARIABLE,             // Variable
	TOKEN_ASSIGNMENT,           // Variable assignment
}   t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum s_ast_node_type
{
	AST_COMMAND,            // A simple command
	AST_PIPE,               // A pipe '|'

	AST_AND,                // Logical AND '&&'
	AST_OR,                 // Logical OR '||'
	AST_SEQUENCE,           // Sequence of commands
	AST_SUBSHELL,           // A subshell '(...)'
	AST_REDIR_OUT,          // Output redirection
	AST_REDIR_OUT_APPEND,   // Output redirection in append mode (>>)
	AST_REDIR_IN,           // Input redirection
	AST_ASSIGNMENT,         // Variable assignment
	AST_HEREDOC,            // Heredoc redirection (<<)
	AST_VARIABLE,           // Variable usage
}   t_ast_node_type;

typedef struct s_ast_node {
	t_ast_node_type     type;           // Node type (command, operator, redirection)
	char                **value;        // Command and arguments (for nodes of type NODE_COMMAND)
	int 				is_last;		// Is the last command in a sequence
	struct s_ast_node   *left;          // Left subtree (command or sub-command)
	struct s_ast_node   *right;         // Right subtree (command or sub-command)
} t_ast_node;

// ########################################################
// #						PARSER						  #
// ########################################################

t_ast_node			*build_ast(t_token **tokens);
t_ast_node			*parse_input(t_minishell *minishell, char *input);

void				process_command(t_token **tokens, t_ast_node **root, t_ast_node **last_command);
void				process_argument(t_token *current, t_ast_node *last_command);
t_ast_node			*process_pipe(t_token **tokens, t_ast_node **root);
t_ast_node			*process_assignment(t_token **tokens);
t_ast_node			*process_variable(t_token **tokens);

t_ast_node			*process_operator(t_token **tokens, t_ast_node **root, t_ast_node **last_command);
void				process_subshell(t_token **tokens, t_ast_node **root, t_ast_node **last_command);
t_ast_node			*process_redirection(t_token **tokens, t_ast_node **root);

// ########################################################
// #					TOKEN_MANAGER					  #
// ########################################################

t_token				*new_token(char *str, int type);
void				add_token(t_token **head, t_token *new);
int					is_token(t_token *token, t_token_type type);

// ########################################################
// #						TOKENIZER					  #
// ########################################################

t_token_type		token_type(char *str);
t_token				*tokenize(char *input);
char				**extract_command_tokens(t_token **tokens);

// ########################################################
// #					PARSER_MANDATORY				  #
// ########################################################

t_ast_node			*extract_full_commands(char *input);
void			*parse_full_commands(t_ast_node *ast);

// ########################################################
// #					PARSER_BONUS					  #
// ########################################################

t_ast_node			*parsing(t_token *tokens);

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

t_ast_node		*new_ast_node(t_ast_node_type type, char **command);
int				is_node(t_ast_node *node, t_ast_node_type type);

// ########################################################
// #						DEBUG						  #
// ########################################################

void			print_tokens(t_token *tokens);
void			print_ast(t_ast_node *ast);

#endif
