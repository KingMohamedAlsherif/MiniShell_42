/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcntl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingmohamedalsherif <kingmohamedalsherif@s +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 04:35:09 by malsheri          #+#    #+#             */
/*   Updated: 2024/09/29 11:13:02 by kingmohamedalshe ###   ########.fr       */
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

void	init_heredoc(t_redir *redir, t_tree_node *n)
{
	char	*line;

	redir->filename = guarantee_file(redir->filename);
	redir->in_fd = open(redir->filename, O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (redir->in_fd < 0)
		ft_error(errno, ft_strdup(redir->filename), n, 1);
	// line = readline("> "); // get_next_line(0);
	while (true)		   // ft_strncmp(redir->heredoc_delim, line, ft_strlen(line) - 1)
	{
		line = readline("> ");										
		// didn't exist
		if (!line)
		{
			// close(redir->in_fd);
			break ;
		}
		//
		if (!ft_strncmp(redir->heredoc_delim, line, ft_strlen(line) - 1)) // used to be inside the loop condition;
		{
			free(line);
			break; 														// Meaning: strcmp would dereference NULL and then
				   														// OOPS! :T
		}
		write(redir->in_fd, line, ft_strlen(line));						  // This has a NULL-check in it, so that's fine.
		write(redir->in_fd, "\n", 1);
		free(line);
	}
	// free(line);
	close(redir->in_fd);
	redir->in_fd = open(redir->filename, O_RDONLY);
}

bool	init_infiles_outfiles(t_redir *redir, t_tree_node *n, int *status)
{
	while (redir)
	{
		if (redir->heredoc_delim)
			init_heredoc(redir, n);
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

void	last_redir_fd(t_redir *redir, char type, int *fd)
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
