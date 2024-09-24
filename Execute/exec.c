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

void	last_redir_fd(t_redir *redir, char type, int *fd)
{
	while (redir->fwd)
		redir = redir->fwd;
	if (type == 'i')
	{
		while (redir->bwd
		&& !redir->in_fd && !redir->heredoc_delim)
			redir = redir->bwd;
		if (redir->in_fd)
			*fd = redir->in_fd;
	}
	else
	{
		while (redir->bwd
		&& !redir->out_fd && !redir->is_append)
			redir = redir->bwd;
		if (redir->out_fd)
			*fd = redir->out_fd;
	}
}

void	init_heredoc(t_redir *redir, t_tree_node *n)
{
	char	*line;

	redir->filename = guarantee_file(redir->filename);
	redir->in_fd = open(redir->filename, O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (redir->in_fd < 0)
		ft_exit(errno, ft_strdup(redir->filename), n, 1);
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
				ft_exit(666, ft_strdup(redir->filename), n, 1);
		}
		else if (redir->is_append)
			redir->out_fd = open(redir->filename,
			O_WRONLY | O_APPEND | O_CREAT, 0777);
		else
			redir->out_fd = open(redir->filename,
			O_WRONLY | O_TRUNC | O_CREAT, 0777);
		if (redir->out_fd < 0)
			ft_exit(errno, ft_strdup(redir->filename), n, 1);
		redir = redir->fwd;
	}
}

void execute(t_tree_node *n, int pipe_index, int pipe_ct)
{
	int		use_in_fd;
	int		use_out_fd;
	t_redir	*redir_ptr;
	// printf("%s\n", n->exec_cmd_path);
	// printf("arg: %s\n", n->cmd_args_arr[0]);
	// printf("%s\n", n->ms->env_arr[0]);
	// printf("pipe idx: %d pipe ct: %d\n", pipe_index, pipe_ct);
	
	redir_ptr = n->redir;
	init_infiles_outfiles(redir_ptr, n);
	use_in_fd = 0;
	use_out_fd = 1;
	if (redir_ptr)
	{
		last_redir_fd(redir_ptr, 'i', &use_in_fd);
		last_redir_fd(redir_ptr, 'o', &use_out_fd);
	}
	if (use_in_fd < 2 && pipe_index)
		use_in_fd = n->pipefd[pipe_index - 1][0];
	if (use_out_fd < 2 && pipe_ct && !n->right)
		use_out_fd = n->pipefd[pipe_index][1];
	// printf("in:%d out:%d\n", use_in_fd, use_out_fd);
	if (dup2(use_in_fd, STDIN_FILENO) < 0)
		ft_exit(errno, ft_strdup("dup infile"), n, 1);
	if (dup2(use_out_fd, STDOUT_FILENO) < 0)
		ft_exit(errno, ft_strdup("dup outfile"), n, 1);
	create_err_file(n);
	close_fds(n, pipe_ct);
	if (is_builtin(n->value))
		execute_builtin(n, n->value);
	else if (!n->exec_cmd_path
		|| execve(n->exec_cmd_path, n->cmd_args_arr, n->ms->env_arr) < 0)
		ft_exit(errno, ft_strdup(n->cmd_args_arr[0]), n, 1);
}

void init_exec(t_tree_node *n, int pipe_ct)
{
	int	pid;
	int	i;
	int	status;

	i = 0;
	while (n->type != END)
	{
		n->pipe_ct = pipe_ct;
		// if ((n->parent && n->parent->type == AND
		// 	&& n->type == CD))
		// 	cd(n);
		if (n->type != PIPE)
		{
			// printf("%s\n", n->exec_cmd_path);
			// printf("%s\n", n->cmd_args[0]);
			pid = fork();
			if (pid < 0)
				ft_exit(errno, ft_strdup("fork"), n, 1);
			if (!pid)
				execute(n, i, pipe_ct);
			i++;
		}
		traverse_tree(&n);
	}
	close_fds(n, pipe_ct);
	while (i-- > 0)
		waitpid(-1, &status, 0);
	update_exit_status(n->ms->env, status);
}
