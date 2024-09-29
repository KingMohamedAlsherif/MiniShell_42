/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malsheri <malsheri@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:19:14 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 04:55:21 by malsheri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lst	*create_new_node(char *str, int ascii_order)
{
	t_lst	*node;
	char	**split_str;

	node = malloc(sizeof(t_lst));
	if (!node)
		return (NULL);
	node->var_n_val = ft_strjoin(str, "\n", 0, 0);
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

void	ft_error(int error, char *str, t_tree_node *n, bool exit_flag)
{
	if (error == 666)
		printf("Minishell: %s: No such file or directory\n", str);
	free(str);
	if (exit_flag)
	{
		close_fds(n, n->pipe_ct);
		free_lst(n->ms->env);
		free_lst(n->ms->exp);
		free_char_arr(n->ms->env_arr, NULL);
		free(n->ms);
		while (n->type != END)
			traverse_tree(&n);
		free_tree(start_node(n));
		exit(1);
	}
}
