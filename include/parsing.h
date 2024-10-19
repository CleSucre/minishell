/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 10:22:57 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/17 10:22:57 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "struct.h"

# define MAX_COMMAND_SIZE 1024

typedef enum s_token_type
{
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_AND_OPERATOR,
	TOKEN_OR_OPERATOR,
	TOKEN_PIPE,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_OUT_APPEND,
	TOKEN_REDIR_IN,
	TOKEN_HEREDOC,
	TOKEN_PARENTHESIS_OPEN,
	TOKEN_PARENTHESIS_CLOSE,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef enum s_ast_node_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_AND,
	AST_OR,
	AST_SUBSHELL,
	AST_REDIR_OUT,
	AST_REDIR_OUT_APPEND,
	AST_REDIR_IN,
	AST_HEREDOC,
}	t_ast_node_type;

typedef struct s_ast_node
{
	t_ast_node_type		type;
	char				**value;
	int					is_last;
	int					in_pipe;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

// ########################################################
// #						PARSER							#
// ########################################################

int				build_ast(t_token **tokens,
					t_ast_node **root, t_ast_node **last_command);
t_ast_node		*parse_input(t_minishell *minishell, char *input);

int				process_command(t_token **tokens, t_ast_node **root,
					t_ast_node **last_command);
int				process_argument(t_token **current, t_ast_node *last_command);
int				process_pipe(t_token **tokens, t_ast_node **root,
					t_ast_node **last_command);
int				run_heredoc(t_minishell *minishell, char *delimiter,
					int *pipes, int *in_out);
int				process_operator(t_token **tokens, t_ast_node **root,
					t_ast_node **last_command);
int				process_subshell(t_token **tokens, t_ast_node **root,
					t_ast_node **last_command);
int				process_heredoc(t_token **tokens, t_ast_node **root,
					t_ast_node **last_command, int is_last);
int				process_redirection(t_token **tokens, t_ast_node **root,
					t_ast_node **last_command, int is_last);
char			*check_input(t_minishell *minishell, char *input);

// ########################################################
// #					TOKEN_MANAGER						#
// ########################################################

t_token			*new_token(char *str, int type);
void			add_token(t_token **head, t_token *new);

// ########################################################
// #						TOKENIZER						#
// ########################################################

t_token_type	token_type(char *str);
t_token			*tokenize(char *input);
char			*extract_token(char *input, int *index);
char			**extract_command_tokens(t_token **tokens);
char			*extract_quoted_token(char *input, int *index);
char			*handle_quotes(char *buffer, int *buffer_pos,
					char *input, int *index);
char			*handle_parentheses(char current_char, int *index);

// ########################################################
// #					PARSER_MANDATORY					#
// ########################################################

t_ast_node		*extract_full_commands(char *input);
void			*parse_full_commands(t_ast_node *ast);

// ########################################################
// #					PARSER_BONUS						#
// ########################################################

t_ast_node		*parsing(t_token *tokens);

// ########################################################
// #						TOKENIZER						#
// ########################################################

t_ast_node_type	token_type_primary(char *str);

// ########################################################
// #					AST_CREATION						#
// ########################################################

void			parse_args(t_ast_node *ast, char **args);

// ########################################################
// #						AST									#
// ########################################################

t_ast_node		*new_ast_node(t_ast_node_type type, char **command);
int				is_node(t_ast_node *node, t_ast_node_type type);

// ########################################################
// #						DEBUG							#
// ########################################################

void			print_tokens(t_token *tokens);
void			print_ast(t_ast_node *ast);

#endif
