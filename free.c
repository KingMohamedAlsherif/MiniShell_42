/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:18:37 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 20:29:22 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_char_arr(char **two_d, char ***three_d)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	if (two_d)
	{
		while (two_d[++i])
			free(two_d[i]);
		free(two_d);
	}
	if (three_d)
	{
		while (three_d[++j])
			free_char_arr(three_d[j], NULL);
		free(three_d);
	}
}

void	free_lst(t_lst *lst)
{
	while (lst->fwd)
	{
		lst = lst->fwd;
		free(lst->bwd->var_n_val);
		free(lst->bwd->var);
		free(lst->bwd->val);
		free(lst->bwd);
	}
	free(lst->var_n_val);
	free(lst->var);
	free(lst->val);
	free(lst);
}

void	free_redir(t_redir *redir)
{
	t_redir	*redir_ptr;

	while (redir->bwd)
		redir = redir->bwd;
	while (redir)
	{
		redir_ptr = redir;
		redir = redir->fwd;
		free(redir_ptr->filename);
		free(redir_ptr->heredoc_delim);
		free(redir_ptr);
	}
}

void	free_tree_node(t_tree_node *n, bool is_read_flag)
{
	printf("node: %d\n", n->type);
	if (n->left && n->left->is_read == is_read_flag)
	{
		printf("left freed: %d\n", n->left->type);
		free(n->left);
		n->left = NULL;
	}
	if (n->right && n->right->is_read == is_read_flag)
	{
		printf("right freed: %d\n", n->right->type);
		free(n->right);
		n->right = NULL;
	}
	if (n->parent && !n->parent->parent && n->parent->is_read == is_read_flag)
	{
		printf("parent freed: %d\n", n->parent->type);
		free(n->parent);
		n->parent = NULL;
	}
}

void	free_tree(t_tree_node *n)
{
	bool	is_read_flag;
	int		i;

	is_read_flag = (n->is_read + 1) % 2;
	i = 0;
	while (i < n->pipe_ct)
		free(n->pipefd[i++]);
	free(n->pipefd);
	while (n && n->type != END)
	{
		if (n->is_read != is_read_flag)
		{
			free(n->value);
			if (n->redir)
				free_redir(n->redir);
			free_char_arr(n->cmd_args_arr, NULL);
			free(n->exec_cmd_path);
		}
		traverse_tree_to_free(&n, is_read_flag);
		if (n)
			free_tree_node(n, is_read_flag);
	}
	printf("last node freed: %d\n", n->type);
	free(n);
}
