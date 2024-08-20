/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_num.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 14:23:16 by chon              #+#    #+#             */
/*   Updated: 2024/03/21 13:18:29 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_count_digits_ull(unsigned long long num, int base)
{
	int	digits;

	digits = 0;
	if (num == 0)
		return (1);
	while (num != 0)
	{
		num /= base;
		digits++;
	}
	return (digits);
}

int	ft_count_digits_u(unsigned int num, int base)
{
	int	digits;

	digits = 0;
	if (num == 0)
		return (1);
	while (num != 0)
	{
		num /= base;
		digits++;
	}
	return (digits);
}

int	ft_count_digits(int num, int base)
{
	int	digits;

	digits = 0;
	if (num == 0)
		return (1);
	if (num < 0)
		digits++;
	while (num != 0)
	{
		num /= base;
		digits++;
	}
	return (digits);
}

int	ft_printf_void_ptr(const char *format, va_list args)
{
	void	*ptr;

	ptr = va_arg(args, void *);
	if (*format == '#')
		return (0);
	ft_putstr_fd("0x", 1);
	ft_putnbr_base_ull((unsigned long long)ptr, "0123456789abcdef");
	return (ft_count_digits_ull((unsigned long long)ptr, 16) + 2);
}

int	ft_printf_num(const char *format, va_list args)
{
	int	nbr;

	nbr = va_arg(args, int);
	if (*format == 'd' || *format == 'i')
	{
		ft_putnbr_base(nbr, "0123456789");
		return (ft_count_digits(nbr, 10));
	}
	else if (*format == 'u')
	{
		ft_putnbr_base_u((unsigned int)nbr, "0123456789");
		return (ft_count_digits_u((unsigned int)nbr, 10));
	}
	else if (*format == 'x')
	{
		ft_putnbr_base_u((unsigned int)nbr, "0123456789abcdef");
		return (ft_count_digits_u((unsigned int)nbr, 16));
	}
	else
	{
		ft_putnbr_base_u((unsigned int)nbr, "0123456789ABCDEF");
		return (ft_count_digits_u((unsigned int)nbr, 16));
	}
}
