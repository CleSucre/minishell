/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quicksort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <mpierrot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:03:19 by mpierrot          #+#    #+#             */
/*   Updated: 2024/10/17 05:48:34 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	swap(char **a, char **b)
{
	char  *tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static int	pivot(char *tabenv[], int min, int max)
{
	int		i;
	int		j;
	char	*pivot;

	pivot = tabenv[max];
	i = min - 1;
	j = min;
	while (j <= max - 1)
	{
		if (ft_strcmp(tabenv[j], pivot) < 0)
		{
			i++;
			swap(&tabenv[i], &tabenv[j]);
		}
		j++;
	}
	swap(&tabenv[i + 1], &tabenv[max]);
	return (i + 1);
}

void  quickSort(char **tabenv, int min, int max)
{
	int	pi;

	if (min < max)
	{
		pi = pivot(tabenv, min, max);
		quickSort(tabenv, min, pi - 1);
		quickSort(tabenv, pi + 1, max);
	}
}
