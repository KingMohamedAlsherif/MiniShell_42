/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/09/23 15:08:47 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*guarantee_file(char *original)
{
	char	*filename;

	filename = ft_strdup(original);
	free(original);
	while (!access(filename, F_OK))
		filename = ft_strjoin(filename, "_new", 1, 0);
	return (filename);
}

void	create_err_file(t_tree_node *n)
{
	char	*err_filename;
	int		err_fd;

	err_filename = guarantee_file(ft_strdup("error"));
	err_fd = open(err_filename, O_WRONLY | O_CREAT);
	if (err_fd < 0)
	{
		unlink(err_filename);
		free(err_filename);
		ft_error(errno, ft_strdup("err file"), n, 1);
	}
	dup2(err_fd, STDERR_FILENO);
	close(err_fd);
	unlink(err_filename);
	free(err_filename);
}

void	close_fds(t_tree_node *n, int pipe_ct)
{
	int	i;

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
	i = -1;
	while (++i < pipe_ct)
	{
		if (n->pipefd[i][0] > 2)
			close(n->pipefd[i][0]);
		if (n->pipefd[i][1] > 2)
			close(n->pipefd[i][1]);
	}
}

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
