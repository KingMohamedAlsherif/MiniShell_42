/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 13:40:43 by chon              #+#    #+#             */
/*   Updated: 2023/12/08 10:58:17 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_p;
	const unsigned char	*src_p;

	dest_p = dest;
	src_p = src;
	if (dest_p == src_p)
		return (dest);
	if (dest_p < src_p)
	{
		while (n > 0)
		{
			*dest_p++ = *src_p++;
			n--;
		}
		return (dest);
	}
	dest_p += n - 1;
	src_p += n - 1;
	while (n > 0)
	{
		*dest_p-- = *src_p--;
		n--;
	}
	return (dest);
}
