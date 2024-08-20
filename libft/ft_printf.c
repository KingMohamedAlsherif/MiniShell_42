/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 13:43:35 by chon              #+#    #+#             */
/*   Updated: 2024/01/25 15:45:20 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	*path(int *count, const char *format, va_list args)
{
	if (*format == 's' || *format == 'c')
		*count += ft_printf_alpha(format, args);
	else if (*format == 'p')
		*count += ft_printf_void_ptr(format, args);
	else if (*format == 'd' || *format == 'i' || *format == 'u'
		|| *format == 'x' || *format == 'X')
		*count += ft_printf_num(format, args);
	else
	{
		ft_putchar_fd(*format, 1);
		(*count)++;
	}
	return (count);
}

int	ft_printf(const char *format, ...)
{
	int		count;
	va_list	args;

	count = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format != '%')
		{
			ft_putchar_fd(*format, 1);
			count++;
		}
		else
		{
			format++;
			path(&count, format, args);
		}
		format++;
	}
	va_end(args);
	return (count);
}
