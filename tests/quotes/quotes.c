/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 09:00:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/06/08 09:00:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Helper function to check if a character is in the charset
static int	ft_is_charset(char c, const char *charset)
{
	while (*charset)
	{
		if (c == *charset)
			return (1);
		charset++;
	}
	return (0);
}

// Helper function to handle quote state
static void	handle_quote(char c, int *in_quotes, char *quote_char)
{
	if ((c == '"' || c == '\'') && (!(*in_quotes) || c == *quote_char))
	{
		*in_quotes = !(*in_quotes);
		*quote_char = c;
	}
}

// Helper function to count the number of words
static int	count_words(const char *str, const char *charset)
{
	int		count;
	int		in_quotes;
	char	quote_char;

	count = 0;
	in_quotes = 0;
	quote_char = '\0';
	while (*str)
	{
		while (*str && ft_is_charset(*str, charset) && !in_quotes)
			str++;
		if (*str)
		{
			count++;
			while (*str && (!ft_is_charset(*str, charset) || in_quotes))
			{
				handle_quote(*str, &in_quotes, &quote_char);
				str++;
			}
		}
	}
	return (count);
}

// Helper function to get the length of a word
static size_t	get_word_length(const char *str, const char *charset)
{
	int		in_quotes;
	char	quote_char;
	size_t	len;

	in_quotes = 0;
	quote_char = '\0';
	len = 0;
	while (*str && (!ft_is_charset(*str, charset) || in_quotes))
	{
		handle_quote(*str, &in_quotes, &quote_char);
		str++;
		len++;
	}
	return (len);
}

// Helper function to copy a word to the result array
static char	*copy_word(const char **str_ptr, const char *charset)
{
	const char	*str;
	const char	*start;
	size_t		len;
	char		*word;

	str = *str_ptr;
	start = str;
	len = get_word_length(str, charset);
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	strncpy(word, start, len);
	word[len] = '\0';
	*str_ptr = start + len;
	return (word);
}

// Helper function to free memory in case of error
static int	ft_freetab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
	return (0);
}

char	**ft_split_quote(char const *str, char const *charset)
{
	int		words;
	char	**result;
	int		i;

	if (!str || !charset)
		return (NULL);
	words = count_words(str, charset);
	result = (char **)malloc((words + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*str)
	{
		while (*str && ft_is_charset(*str, charset))
			str++;
		if (*str)
		{
			result[i] = copy_word(&str, charset);
			if (!result[i])
				return ((char **)ft_freetab(result));
			i++;
		}
	}
	result[i] = NULL;
	return (result);
}

int	main(void)
{
	char	*str;
	char	*charset;
	char	**result;
	int		i;

	str = strdup("Hello \"world, welcome\" 'to the ' world | of' \"programming. Its a very fun place.");
	charset = strdup(" ,|");
	result = ft_split_quote(str, charset);
	if (result)
	{
		i = 0;
		while (result[i])
		{
			printf("Word[%d]: %s\n", i, result[i]);
			free(result[i]);
			i++;
		}
		free(result);
	}
	else
		printf("Error in splitting the string.\n");
	free(str);
	free(charset);
	return (0);
}
