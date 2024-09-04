/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:36:20 by chon              #+#    #+#             */
/*   Updated: 2024/09/04 15:26:37 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	insert_node(t_tree_node *n, char *str)
{
	t_lst	*env_node;
	t_lst	*export_node;
	t_lst	*tmp_node;
	int		rank;

	env_node = n->ms_env;
	while (env_node->next)
		env_node = env_node->next;
	env_node->next = new_node(ft_strdup(str), 0);
	update_order(n->ms_env, env_node);
	rank = env_node->ascii_order;
	export_node = n->ms_export;
	while (export_node->ascii_order < rank - 1)
		export_node = export_node->next;
	tmp_node = export_node->next;
	export_node->next = new_node(export_str(str), rank);
	export_node->next->next = tmp_node;
	export_node = tmp_node;
	while(export_node)
	{
		export_node->ascii_order++;
		export_node = export_node->next;
	}
}

int	has_valid_chars(char *str)
{
	while (*str)
	{
		if (*str < 'A' || *str > 'z')
			return (0);
		str++;
	}
	return (1);
}
