/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:17:25 by chon              #+#    #+#             */
/*   Updated: 2024/07/08 09:17:25 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_heredoc(t_redir *redir, t_tree_node *n)
{
	char	*line;

	if (!access(redir->filename, F_OK))
		redir->filename = ft_strjoin(redir->filename, ft_strdup("_tmp"));
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
	// printf("%d\n", n->out_fd);
}

void	init_infiles_outfiles(t_redir *redir, t_tree_node *n)
{
	while (redir)
	{
		if (redir->heredoc_delim)
			init_heredoc(redir, n);
		else if (redir->in_fd)
		{
			redir->in_fd = open(redir->filename, O_RDONLY);
			if (redir->in_fd < 0)
				ft_error(errno, redir->filename, n, 0);
		}
		else if (redir->is_append)
			redir->out_fd = open(redir->filename, O_WRONLY | O_APPEND | O_CREAT, 0777);
		else
			redir->out_fd = open(redir->filename, O_WRONLY | O_TRUNC | O_CREAT, 0777);
		if (redir->out_fd < 0)
			ft_error(errno, ft_strdup(redir->filename), n, 1);
		redir = redir->fwd;
	}
	// printf("%d\n", n->redir->in_fd);
}

void execute(t_tree_node *n, int pipe_index, int pipe_ct)
{
	int	use_in_fd;
	int	use_out_fd;

	// printf("%s\n", n->exec_cmd_path);
	// printf("%s\n", n->cmd_args_arr[0]);
	// printf("%s\n", n->ms->env_arr[0]);
	// printf("%d\n", pipe_index);
	init_infiles_outfiles(n->redir, n);
	use_in_fd = 0;
	use_out_fd = 1;
	if (n->redir)
	{
		use_in_fd = last_redir_fd(n->redir, 'i');
		use_out_fd = last_redir_fd(n->redir, 'o');
	}
	if (pipe_index > 0 && !use_in_fd)
		use_in_fd = n->pipefd[pipe_index - 1][0];
	if (n->right && n->right->type == END && !use_out_fd)
		use_out_fd = 1;
	else if (pipe_index > 0)
		use_out_fd = n->pipefd[pipe_index][1];
	// printf("in:%d out:%d\n", use_in_fd, use_out_fd);
	if (dup2(use_in_fd, STDIN_FILENO) < 0)
		ft_error(errno, ft_strdup("dup infile"), n, 1);
	if (dup2(use_out_fd, STDOUT_FILENO) < 0)
		ft_error(errno, ft_strdup("dup outfile"), n, 1);
	close_fds(n, pipe_ct);
	if (execve(n->exec_cmd_path, n->cmd_args_arr, n->ms->env_arr) < 0)
		ft_error(errno, ft_strdup("execve"), n, 1);
}

void init_exec(t_tree_node *n, int pipe_ct)
{
	t_tree_node *n_head;
	int			pid;
	int			i;

	n_head = n;
	i = 0;
	while (n->type != END)
	{
		if ((n->parent && n->parent->type == AND
			&& n->type == CD))
			cd(n);
		if (n->type != PIPE)
		{
			// printf("%s\n", n->exec_cmd_path);
			// printf("%s\n", n->cmd_args[0]);
			pid = fork();
			if (pid < 0)
				ft_error(errno, ft_strdup("fork"), n, 1);
			if (!pid)
				execute(n, i, pipe_ct);
			waitpid(pid, NULL, WNOHANG);
			i++;
		}
		traverse_tree(&n, 1);
	}
	close_fds(n_head, pipe_ct);
}
