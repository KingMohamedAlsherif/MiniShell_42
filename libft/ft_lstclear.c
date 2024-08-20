/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 14:55:15 by chon              #+#    #+#             */
/*   Updated: 2023/12/13 16:40:03 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*cur;
	t_list	*next;

	if (!lst || !*lst || !del)
		return ;
	cur = *lst;
	while (cur)
	{
		next = cur->next;
		del((cur)->content);
		free(cur);
		cur = next;
	}
	*lst = NULL;
}
