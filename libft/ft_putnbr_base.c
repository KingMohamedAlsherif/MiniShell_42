/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:43:42 by chon              #+#    #+#             */
/*   Updated: 2024/01/25 15:25:10 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	convert(int nbr, char *base, int len)
{
	if (nbr < 0)
	{
		write(1, "-", 1);
		if (nbr == -2147483648)
		{
			convert(-2147483648 / len * -1, base, len);
			convert(-2147483648 % len * -1, base, len);
		}
		else
			convert(-nbr, base, len);
	}
	else if (nbr > len - 1)
	{
		convert(nbr / len, base, len);
		convert(nbr % len, base, len);
	}
	else if (nbr < len)
		write(1, &base[nbr], 1);
}

void	ft_putnbr_base(int nbr, char *base)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (base[i])
	{
		len++;
		i++;
	}
	convert(nbr, base, len);
}
