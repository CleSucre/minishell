/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 06:54:17 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/08 06:54:17 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_print_param(va_list ap, const char param, int fd)
{
	if (param == 'c')
		return (ft_putchar_fd(va_arg(ap, int), fd));
	else if (param == 's')
		return (ft_putstr_fd(va_arg(ap, char *), fd));
	else if (param == 'p')
		return (ft_putpointer_fd(va_arg(ap, char *), fd));
	else if (param == 'd' || param == 'i')
		return (ft_putnbr_fd(va_arg(ap, int), fd));
	else if (param == 'u')
		return (ft_putnbr_unsigned_fd(va_arg(ap, unsigned int), fd));
	else if (param == 'f')
		return (ft_putnbr_float_fd(va_arg(ap, double), fd));
	else if (param == 'x')
		return (ft_putnbr_base_fd(
				va_arg(ap, unsigned int), "0123456789abcdef", fd));
	else if (param == 'X')
		return (ft_putnbr_base_fd(
				va_arg(ap, unsigned int), "0123456789ABCDEF", fd));
	else if (param == '%')
		return (ft_putchar_fd('%', fd));
	return (ft_putchar_fd(param, fd));
}

int	ft_fprintf(int fd, const char *format, ...)
{
	va_list	param;
	int		res;

	if (!format)
		return (-1);
	res = 0;
	va_start(param, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			res += ft_print_param(param, *format, fd);
		}
		else
			res += ft_putchar_fd(*format, fd);
		if (res < 0)
		{
			va_end(param);
			return (-1);
		}
		format++;
	}
	va_end(param);
	return (res);
}

int	ft_printf(const char *format, ...)
{
	va_list	param;
	int		res;

	if (!format)
		return (-1);
	res = 0;
	va_start(param, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			res += ft_print_param(param, *format, STDOUT_FILENO);
		}
		else
			res += ft_putchar_fd(*format, STDOUT_FILENO);
		if (res < 0)
		{
			va_end(param);
			return (-1);
		}
		format++;
	}
	va_end(param);
	return (res);
}
