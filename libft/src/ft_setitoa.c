/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setitoa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 05:37:52 by julthoma          #+#    #+#             */
/*   Updated: 2024/04/27 05:37:52 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void	ft_insert(char *dst, long n, int *i)
{
	if (n > 9)
	{
		ft_insert(dst, n / 10, i);
		ft_insert(dst, n % 10, i);
	}
	else
		dst[(*i)++] = n + '0';
}

void	ft_setitoa(char *dst, int n)
{
	long	num;
	int		i;

	num = n;
	i = 0;
	if (num < 0)
	{
		dst[i++] = '-';
		num *= -1;
	}
	ft_insert(dst, num, &i);
	dst[i] = '\0';
}
