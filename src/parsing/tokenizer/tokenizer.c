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

t_token_type	token_type(char *str)
{

}

t_token	*tokenize(char *input)
{
	t_token			*token;
	t_token			*head;
	t_token_type	type;
	char 			**splited;
	int				i;

	splited = ft_split(input, WHITESPACES);
	if (splited == NULL)
		return (NULL);
	i = 0;
	head = NULL;
	while (splited[i])
	{
		type = token_type(splited[i]);
		token = new_token(splited[i], type);
		add_token(&head, token);
		i++;
	}
	return (head);
}