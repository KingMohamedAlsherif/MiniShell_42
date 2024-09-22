/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:19:14 by chon              #+#    #+#             */
/*   Updated: 2024/09/09 16:43:31 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst	*create_new_node(char *str, int ascii_order)
{
	t_lst	*node;
	char	**split_str;

	node = malloc(sizeof(t_lst));
	if (!node)
		exit (1);
	node->var_n_val = ft_strdup(str);
	split_str = ft_split(str, '=');
	free(str);
	node->var = ft_strdup(split_str[0]);
	if (split_str[1])
		node->val = ft_strdup(split_str[1]);
	else
		node->val = NULL;
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

void ft_error(int error, char *str, t_tree_node *n, int exit_switch)
{
	printf("error #: %d\n", error);
	if (!error)
		ft_printf("%s\n", str);
	if (error == 14 || error == 2)
		ft_printf("%s: command not found\n", str);
	else if (error == 666)
		ft_printf("-Minishell: %s: No such file or directory\n", str);
	else
		ft_printf("%s: %s\n", str, strerror(error));
	free(str);
	free_tree(start_node(n));
	if (exit_switch)
		exit(EXIT_FAILURE);
}
