/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 13:40:33 by chon              #+#    #+#             */
/*   Updated: 2023/12/08 10:18:17 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	digits(int n)
{
	int	dig;

	dig = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		dig++;
	while (n != 0)
	{
		n /= 10;
		dig++;
	}
	return (dig);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	long	num;

	num = n;
	i = digits(n);
	str = ft_calloc(sizeof(char), (i + 1));
	if (str == NULL)
		return (NULL);
	if (n == 0)
		str[0] = '0';
	if (n < 0)
	{
		str[0] = '-';
		num = (long)n * -1;
	}
	str[i--] = '\0';
	while (num > 0)
	{
		str[i--] = num % 10 + 48;
		num /= 10;
	}
	return (str);
}
