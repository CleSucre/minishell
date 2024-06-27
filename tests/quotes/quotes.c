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

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

static char	**initialize_result(int capacity)
{
	char	**result;

	result = (char **)malloc(capacity * sizeof(char *));
	if (!result)
		return (NULL);
	return (result);
}

static int	resize_result(char ***result, int *capacity)
{
	int		new_capacity;
	char	**new_result;

	new_capacity = *capacity * 2;
	new_result = (char **)realloc(*result, new_capacity * sizeof(char *));
	if (!new_result)
		return (0);
	*result = new_result;
	*capacity = new_capacity;
	return (1);
}

static void	free_result(char **result, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(result[i]);
		i++;
	}
	free(result);
}

static char	*create_word(const char *start, int length)
{
	char	*word;

	word = (char *)malloc((length + 1) * sizeof(char));
	if (!word)
		return (NULL);
	strncpy(word, start, length);
	word[length] = '\0';
	return (word);
}

static char	**process_string(const char *str, const char *charset,
		const char *quote_set, char **result)
{
	int			count;
	char		current_quote;
	const char	*start;
	int			capacity;

	count = 0;
	current_quote = 0;
	capacity = 10;
	while (*str)
	{
		while (*str && !current_quote && strchr(charset, *str))
			str++;
		if (!*str)
			break ;
		start = str;
		while (*str && (current_quote || !strchr(charset, *str)))
		{
			if (!current_quote && strchr(quote_set, *str))
				current_quote = *str;
			else if (current_quote == *str)
				current_quote = 0;
			str++;
		}
		if (count == capacity && !resize_result(&result, &capacity))
		{
			free_result(result, count);
			return (NULL);
		}
		result[count++] = create_word(start, str - start);
	}
	result[count] = NULL;
	return (result);
}

/**
 * @brief Split the string into words,
 * 		if the word is quoted, don't split what's inside.
 *
 * @param str The string to split.
 * @param charset The set of delimiter characters.
 * @return A dynamically allocated array of strings, NULL-terminated.
 */
char	**ft_split_quote(const char *str,
		const char *charset, const char *quote_set)
{
	char	**result;

	result = initialize_result(10);
	if (!result)
		return (NULL);
	result = process_string(str, charset, quote_set, result);
	return (result);
}

static int	ft_check(char **res, char **expected)
{
	int	i;

	i = 0;
	while (res[i] && expected[i])
	{
		if (strcmp(res[i], expected[i]))
			return (0);
		i++;
	}
	if (res[i] || expected[i])
		return (0);
	return (1);
}

static int	ft_test(char *str, char *charset, char *quote_set, char **expected)
{
	int 	succes;
	char	**res;

	res = ft_split_quote(str, charset, quote_set);
	if (ft_check(res, expected))
	{
		printf(COLOR_GREEN "OK : " COLOR_GREEN);
		succes = 1;
	}
	else
	{
		printf(COLOR_RED "KO : " COLOR_RED);
		succes = 0;
	}
	printf("str: |%s| -> ", str);
	for (int i = 0; res[i]; i++)
	{
		printf("[%s]", res[i]);
		free(res[i]);
	}
	if (!succes)
	{
		printf(" EXPECTED : ");
		for (int i = 0; expected[i]; i++)
			printf("[%s]", expected[i]);
	}
	printf("\n");
	free(res);
	return (succes);
}

int	main(void)
{
	int	succes;

	succes = 0;
	succes += ft_test("hello \"world\" !", " ", "\"'", (char *[]){"hello", "\"world\"", "!", NULL});
	succes += ft_test("\"hello \"world\" !\"", " ", "\"'", (char *[]){ "\"hello \"world\" !\"", NULL});
	succes += ft_test("hello\"world\"!", " ", "\"", (char *[]){"hello\"world\"!", NULL});
	succes += ft_test("\"hello 'world' !\"", " ", "\"", (char *[]){"\"hello 'world' !\"", NULL});
	succes += ft_test("hello 'world' !", " ", "\"'", (char *[]){"hello", "'world'", "!", NULL});
	succes += ft_test("hello 'wo\"rld' !", " ", "\"", (char *[]){"hello", "'wo\"rld' !", NULL});

	printf(COLOR_RESET"Total : %d/6\n", succes);
	return (0);
}
