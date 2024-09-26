#include "../minishell.h"

void	init_heredoc(t_redir *redir, t_tree_node *n)
{
	char	*line;

	redir->filename = guarantee_file(redir->filename);
	redir->in_fd = open(redir->filename, O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (redir->in_fd < 0)
		ft_error(errno, ft_strdup(redir->filename), n, 1);
	line = get_next_line(0);
	while (ft_strncmp(redir->heredoc_delim, line, ft_strlen(line) - 1))
	{
		write(redir->in_fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	free(line);
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
