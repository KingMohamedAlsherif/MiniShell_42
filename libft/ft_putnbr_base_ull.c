/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_ull.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 14:43:42 by chon              #+#    #+#             */
/*   Updated: 2024/01/25 15:25:07 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	convert_ull(unsigned long long nbr, char *base, unsigned long long len)
{
	if (nbr > len - 1)
	{
		convert_ull(nbr / len, base, len);
		convert_ull(nbr % len, base, len);
	}
	else if (nbr < len)
		write(1, &base[nbr], 1);
}

void	ft_putnbr_base_ull(unsigned long long nbr, char *base)
{
	unsigned long long	len;
	int					i;

	len = 0;
	i = 0;
	while (base[i])
	{
		len++;
		i++;
	}
	convert_ull(nbr, base, len);
}
