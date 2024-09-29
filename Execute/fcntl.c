/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcntl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:45:37 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 15:45:37 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_exit_status(t_lst *env, int status)
{
	while (env && ft_strncmp(env->var, "?", 2))
		env = env->fwd;
	free(env->val);
	env->val = (ft_itoa(status));
}

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

void	init_heredoc(t_redir *redir)
{
	char	*line_tracker;
	char	*line;

	line_tracker = NULL;
	line = readline("> ");
	while (ft_strncmp(redir->heredoc_delim, line, ft_strlen(line) - 1))
	{
		line_tracker = ft_strjoin(line_tracker, line, 1, 1);
		line_tracker = ft_strjoin(line_tracker, "\n", 1, 0);
		line = readline("> ");
	}
	free(line);
	redir->filename = guarantee_file(redir->filename);
	redir->in_fd = open(redir->filename, O_RDWR | O_CREAT, 0620);
	write(redir->in_fd, line_tracker, ft_strlen(line_tracker));
	free(line_tracker);
	close(redir->in_fd);
	redir->in_fd = open(redir->filename, O_RDONLY);
}

bool	init_infiles_outfiles(t_redir *redir, t_tree_node *n, int *status)
{
	while (redir)
	{
		if (redir->heredoc_delim)
			init_heredoc(redir);
		else if (redir->in_fd)
		{
			redir->in_fd = open(redir->filename, O_RDONLY);
			if (redir->in_fd < 0)
			{
				*status = 1;
				traverse_tree(&n);
				return (ft_error(666, ft_strdup(redir->filename), n, 0), 0);
			}
		}
		else if (redir->is_append)
			redir->out_fd = open(redir->filename,
			O_WRONLY | O_APPEND | O_CREAT, 0777);
		else
			redir->out_fd = open(redir->filename,
			O_WRONLY | O_TRUNC | O_CREAT, 0777);
		if (redir->out_fd < 0)
			return (ft_error(errno, ft_strdup(redir->filename), n, 0), 0);
		redir = redir->fwd;
	}
	return (1);
}
