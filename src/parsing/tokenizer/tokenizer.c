/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cmp_token(char **tokens, int i,
				int *parentheses_balance, int token_count)
{
	if (ft_strcmp(tokens[i], "(") == 0)
	{
		(*parentheses_balance)++;
		if (i + 1 < token_count && ft_strcmp(tokens[i + 1], ")") == 0)
		{
			printf("syntax error near unexpected token `)'\n");
			return (0);
		}
	}
	else if (ft_strcmp(tokens[i], ")") == 0)
	{
		(*parentheses_balance)--;
		if (*parentheses_balance < 0)
		{
			printf("syntax error near unexpected token `)'\n");
			return (0);
		}
	}
	return (1);
}

static int	are_parentheses_valid(char **tokens, int token_count)
{
	int	i;
	int	parentheses_balance;

	i = 0;
	parentheses_balance = 0;
	while (i < token_count)
	{
		if (!cmp_token(tokens, i, &parentheses_balance, token_count))
			return (0);
		i++;
	}
	if (parentheses_balance != 0)
	{
		printf("syntax error: unclosed parentheses\n");
		return (0);
	}
	return (1);
}

static void	cmp_token_list(t_token **token_list, char **tokens, int i)
{
	if (ft_strcmp(tokens[i], "&&") == 0)
		add_token_to_list(token_list, TOKEN_AND_OPERATOR, tokens[i]);
	else if (ft_strcmp(tokens[i], "||") == 0)
		add_token_to_list(token_list, TOKEN_OR_OPERATOR, tokens[i]);
	else if (ft_strcmp(tokens[i], "<<") == 0)
		add_token_to_list(token_list, TOKEN_HEREDOC, tokens[i]);
	else if (ft_strcmp(tokens[i], ">>") == 0)
		add_token_to_list(token_list, TOKEN_REDIR_OUT_APPEND, tokens[i]);
	else if (ft_strcmp(tokens[i], ">") == 0)
		add_token_to_list(token_list, TOKEN_REDIR_OUT, tokens[i]);
	else if (ft_strcmp(tokens[i], "<") == 0)
		add_token_to_list(token_list, TOKEN_REDIR_IN, tokens[i]);
	else if (ft_strcmp(tokens[i], "|") == 0)
		add_token_to_list(token_list, TOKEN_PIPE, tokens[i]);
	else if (ft_strcmp(tokens[i], "(") == 0)
		add_token_to_list(token_list, TOKEN_PARENTHESIS_OPEN, tokens[i]);
	else if (ft_strcmp(tokens[i], ")") == 0)
		add_token_to_list(token_list, TOKEN_PARENTHESIS_CLOSE, tokens[i]);
	else
		add_token_to_list(token_list, TOKEN_COMMAND, tokens[i]);
}

void	tokenize(const char *input, t_token **token_list)
{
	int		token_count;
	char	**tokens;
	int		i;

	token_count = 0;
	ft_fprintf(2, "DEBUG: input: %s\n", input);
	tokens = split_with_quotes(input, &token_count);
	ft_fprintf(2, "DEBUG: input 2: %s\n", input);
	if (!are_parentheses_valid(tokens, token_count))
	{
		return (clear_token_list(token_list, tokens, token_count));
	}
	i = 0;
	while (i < token_count)
	{
		cmp_token_list(token_list, tokens, i);
		i++;
	}
	i = 0;
	while (i < token_count)
		free(tokens[i++]);
	free(tokens);
}

char	**extract_command_tokens(t_token **tokens)
{
	char	**command_tokens;
	int		token_count;
	t_token	*current;

	command_tokens = (char **)malloc(MAX_COMMAND_SIZE * sizeof(char *));
	if (command_tokens == NULL)
		return (NULL);
	token_count = 0;
	current = *tokens;
	while (current != NULL
		&& (current->type == TOKEN_COMMAND || current->type == TOKEN_ARGUMENT))
	{
		command_tokens[token_count++] = ft_strdup(current->value);
		current = current->next;
	}
	command_tokens[token_count] = NULL;
	*tokens = current;
	return (command_tokens);
}
