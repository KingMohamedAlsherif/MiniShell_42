/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 13:41:10 by chon              #+#    #+#             */
/*   Updated: 2024/06/25 13:52:05 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (s1_len + s2_len == 0)
		return (NULL);
	str = malloc(s1_len + s2_len + 1);
	if (!str)
		return (NULL);
	i = 0;
	ft_memcpy(str, s1, s1_len);
	i += s1_len;
	ft_memcpy(str + i, s2, s2_len);
	i += s2_len;
	str[i] = '\0';
	free(s1);
	free(s2);
	return (str);
}
