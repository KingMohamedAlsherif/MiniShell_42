/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:18:37 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 23:53:16 by chon             ###   ########.fr       */
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

void	free_tree_node(t_tree_node *n)
{
	free(n->value);
	if (n->redir)
		free_redir(n->redir);
	free_char_arr(n->cmd_args_arr, NULL);
	free(n->exec_cmd_path);
	free(n);
}

void	free_tree(t_tree_node *n)
{
	int		i;
	t_tree_node	*tmp;

	i = 0;
	while (i < n->pipe_ct)
		free(n->pipefd[i++]);
	free(n->pipefd);
	if (n->right && n->right->type == END)
	{
		free_tree_node(n->right);
		free_tree_node(n);
	}
	else
	{
		while (n->parent)
		{
			tmp = n;
			n = n->parent;
			free_tree_node(tmp);
			if (n->right->right)
				free_tree_node(n->right->right);
			free_tree_node(n->right);
		}
		free_tree_node(n);
	}
}
