/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/09/28 23:42:39 by chon             ###   ########.fr       */
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

