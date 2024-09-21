/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/09/13 19:34:14 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if ((*n)->parent && ((!(*n)->left && !(*n)->right)
		|| ((*n)->left && (*n)->left->is_read == read_flag
			&& (*n)->right && (*n)->right->is_read == read_flag)))
		*n = (*n)->parent;
	else if ((*n)->left && (*n)->left->is_read == unread_flag)
		*n = (*n)->left;
	else
		*n = (*n)->right;
}

int is_empty(char *av)
{
	int i;
	size_t sp_ct;

	i = -1;
	sp_ct = 0;
	while (av[++i])
	{
		if (av[i] == ' ')
			sp_ct++;
	}
	if (sp_ct == ft_strlen(av))
		return (0);
	return (1);
}

void	close_fds(t_tree_node *n, int pipe_ct)
{
	int			i;

	i = -1;
	if (n->type != END)
		while (n->redir)
		{
			if (n->redir->in_fd > 2)
				close(n->redir->in_fd);
			if (n->redir->out_fd > 2)
				close(n->redir->out_fd);
			if (n->redir->heredoc_delim)
				unlink(n->redir->filename);
			n->redir = n->redir->fwd;
		}
	if (n->type == END)
		n = start_node(n);
	while (++i < pipe_ct)
	{
		close(n->pipefd[i][0]);
		close(n->pipefd[i][1]);
		free(n->pipefd[i]);
	}
	free(n->pipefd);
}
