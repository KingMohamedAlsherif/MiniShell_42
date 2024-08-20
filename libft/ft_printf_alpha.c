/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_alpha.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 14:15:50 by chon              #+#    #+#             */
/*   Updated: 2024/01/25 15:45:01 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf_alpha(const char *format, va_list args)
{
	int		count;
	char	*str;

	count = 0;
	if (*format == 's')
	{
		str = va_arg(args, char *);
		if (str == NULL)
		{
			count += 6;
			ft_putstr_fd("(null)", 1);
		}
		else
		{
			ft_putstr_fd(str, 1);
			count += ft_strlen(str);
		}
	}
	else
	{
		ft_putchar_fd(va_arg(args, int), 1);
		count++;
	}
	return (count);
}
