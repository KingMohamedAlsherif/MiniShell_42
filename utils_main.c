/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:19:14 by chon              #+#    #+#             */
/*   Updated: 2024/09/06 12:22:53 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst	*new_node(char *str, int ascii_order)
{
	t_lst	*node;
	char	**split_str;

	node = malloc(sizeof(t_lst));
	if (!node)
		exit (1);
	split_str = ft_split(str, '=');
	node->var = ft_strdup(split_str[0]);
	if (split_str[1])
		node->str = ft_strdup(split_str[1]);
	else
		node->str = NULL;
	node->ascii_order = ascii_order;
	free_char_arr(split_str, NULL);
	node->fwd = NULL;
	node->bwd = NULL;
	return (node);
}

int	is_number(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	count_lst_nodes(t_lst *head)
{
	int	node_ct;

	node_ct = 0;
	while (head)
	{
		node_ct++;
		head = head->fwd;
	}
	return (node_ct);
}
