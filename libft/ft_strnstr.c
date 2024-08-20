/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 13:41:26 by chon              #+#    #+#             */
/*   Updated: 2023/12/23 11:22:00 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t n)
{
	int	i;
	int	j;

	if (ft_strlen(needle) == 0)
		return ((char *)haystack);
	if (n == 0 || (ft_strlen(haystack) == 0 && ft_strlen(needle) != 0))
		return (0);
	i = 0;
	j = 0;
	if (n > ft_strlen(haystack))
		n = ft_strlen(haystack);
	while (haystack[i] && n > 0)
	{
		while (haystack[i + j] == needle[j] && n - j > 0)
			j++;
		if ((size_t)j == ft_strlen(needle))
			return ((char *)(haystack + i));
		j = 0;
		n--;
		i++;
	}
	return (NULL);
}
