/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcntl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingmohamedalsherif <kingmohamedalsherif@s +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 04:35:09 by malsheri          #+#    #+#             */
/*   Updated: 2024/09/29 18:20:36 by kingmohamedalshe ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t sig;

void update_exit_status(t_lst *env, int status)
{
	if (sig == 1)
	{
		status = 1;
		sig = 0;
	}
	while (env && ft_strncmp(env->var, "?", 2))
		env = env->fwd;
	free(env->val);
	env->val = (ft_itoa(status));
}

void init_heredoc(t_redir *redir)
{
	char	*line_tracker;
	char	*line;

	line_tracker = NULL;
	line = readline("> ");
	if (line)
	{
		while (ft_strncmp(redir->heredoc_delim, line, ft_strlen(line) - 1))
		{
			line_tracker = ft_strjoin(line_tracker, line, 1, 1);
			line_tracker = ft_strjoin(line_tracker, "\n", 1, 0);
			line = readline("> ");
			if (!line)
				break ;
		}
		free(line);
		redir->filename = guarantee_file(redir->filename);
		redir->in_fd = open(redir->filename, O_RDWR | O_CREAT, 0620);
		write(redir->in_fd, line_tracker, ft_strlen(line_tracker));
		free(line_tracker);
		close(redir->in_fd);
		redir->in_fd = open(redir->filename, O_RDONLY);
	}
}

bool init_infiles_outfiles(t_redir *redir, t_tree_node *n)
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
				sig = 1;
				traverse_tree(&n);
				return (ft_exit(666, ft_strdup(redir->filename), n, 0), 0);
			}
		}
		else if (redir->is_append)
			redir->out_fd = open(redir->filename, O_WRONLY | O_APPEND | O_CREAT,
								 0777);
		else
			redir->out_fd = open(redir->filename, O_WRONLY | O_TRUNC | O_CREAT,
								 0777);
		if (redir->out_fd < 0)
			return (ft_exit(errno, ft_strdup(redir->filename), n, 0), 0);
		redir = redir->fwd;
	}
	return (1);
}

void last_redir_fd(t_redir *redir, char type, int *fd)
{
	while (redir->fwd)
		redir = redir->fwd;
	if (type == 'i')
	{
		while (redir->bwd && !redir->in_fd && !redir->heredoc_delim)
			redir = redir->bwd;
		if (redir->in_fd)
			*fd = redir->in_fd;
	}
	else
	{
		while (redir->bwd && !redir->out_fd && !redir->is_append)
			redir = redir->bwd;
		if (redir->out_fd)
			*fd = redir->out_fd;
	}
}
