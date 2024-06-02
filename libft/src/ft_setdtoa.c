/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setdtoa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 05:28:05 by julthoma          #+#    #+#             */
/*   Updated: 2024/04/27 05:28:05 by julthoma         ###   ########.fr       */
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

static int	ft_topositive(char *dst, long *num, double *n)
{
	if (*n < 0)
	{
		dst[0] = '-';
		*num = -*num;
		*n = -*n;
		return (1);
	}
	return (0);
}

void	ft_setdtoa(char *dst, double n, int precision)
{
	long	num;
	int		i;

	num = (long)n;
	n = n - num;
	i = ft_topositive(dst, &num, &n);
	ft_insert(dst, num, &i);
	dst[i++] = '.';
	while (precision--)
	{
		n *= 10;
		num = (long)n;
		dst[i++] = num + '0';
		n -= num;
	}
	dst[i] = '\0';
}
