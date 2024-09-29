/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mv_tree.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 04:56:22 by malsheri          #+#    #+#             */
/*   Updated: 2024/09/30 01:58:48 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree_node	*start_node(t_tree_node *n)
{
	while (n->parent)
		n = n->parent;
	while (n->left)
		n = n->left;
	return (n);
}

void	traverse_tree(t_tree_node **n)
{
	bool	read_flag;
	bool	unread_flag;

	if ((start_node(*n)) == *n)
		read_flag = ((*n)->is_read + 1) % 2;
	else
		read_flag = start_node(*n)->is_read;
	unread_flag = (read_flag + 1) % 2;
	(*n)->is_read = read_flag;
	if ((*n)->type == END)
		return ;
	if ((*n)->parent && ((!(*n)->left && !(*n)->right) || ((*n)->left
				&& (*n)->left->is_read == read_flag && (*n)->right
				&& (*n)->right->is_read == read_flag)))
		*n = (*n)->parent;
	else if ((*n)->left && (*n)->left->is_read == unread_flag)
		*n = (*n)->left;
	else
		*n = (*n)->right;
}

void	traverse_and_check_errors(t_tree_node *n)
{
	while (n->type != END)
	{
		if ((n->exec_cmd_path && !ft_strncmp(n->exec_cmd_path, "?", 2)
				&& !is_builtin(n->value, n->redir)) || (!is_builtin(n->value,
					n->redir) && n->exec_cmd_path && !strncmp(n->exec_cmd_path,
					"invalid", 8)))
			printf("%s: command not found\n", n->value);
		else if (n->value && !is_builtin(n->value, n->redir)
			&& !strncmp(n->exec_cmd_path, "PATH", 5))
			printf("Minishell: %s: No such file or directory\n", n->value);
		traverse_tree(&n);
	}
}
