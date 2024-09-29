/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 14:42:12 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_pipe(int **pipe, int pipe_ct)
{
	int		i;

	i = -1;
	while (++i < pipe_ct)
	{
		if (pipe[i][0] > 2)
			close(pipe[i][0]);
		if (pipe[i][1] > 2)
			close(pipe[i][1]);
	}
}

void	close_fds(t_tree_node *n, int pipe_ct)
{
	t_redir	*redir_ptr;

	close_pipe(start_node(n)->pipefd, pipe_ct);
	while (n->type != END)
	{
		redir_ptr = n->redir;
		while (redir_ptr)
		{
			if (redir_ptr->in_fd > 2)
				close(redir_ptr->in_fd);
			if (redir_ptr->out_fd > 2)
				close(redir_ptr->out_fd);
			if (redir_ptr->heredoc_delim)
				unlink(redir_ptr->filename);
			redir_ptr = redir_ptr->fwd;
		}
		traverse_tree(&n);
	}
}

