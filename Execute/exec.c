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

void execute(t_tree_node *n, int pipe_index, int pipe_ct)
{
	int	use_in_fd;
	int	use_out_fd;
	// printf("%s\n", n->value);
	// printf("exec path: %s\n", n->exec_cmd_path);
	// printf("arg: %s\n", n->cmd_args_arr[0]);
	// printf("pipe idx: %d pipe ct: %d\n", pipe_index, pipe_ct);
	
	use_in_fd = 0;
	use_out_fd = 1;
	if (n->redir)
	{
		last_redir_fd(n->redir, 'i', &use_in_fd);
		last_redir_fd(n->redir, 'o', &use_out_fd);
	}
	if (use_in_fd < 2 && pipe_index)
		use_in_fd = n->pipefd[pipe_index - 1][0];
	if (use_out_fd < 2 && pipe_ct && !n->right)
		use_out_fd = n->pipefd[pipe_index][1];
	printf("in:%d out:%d\n", use_in_fd, use_out_fd);
	if (dup2(use_in_fd, STDIN_FILENO) < 0)
		ft_error(errno, ft_strdup("dup infile"), n, 1);
	if (dup2(use_out_fd, STDOUT_FILENO) < 0)
		ft_error(errno, ft_strdup("dup outfile"), n, 1);
	create_err_file(n);
	close_fds(n, pipe_ct);
	if (ft_strlen(n->value) == 3 && !ft_strncmp(n->value, "env", 4)
		&& n->cmd_args_arr && n->cmd_args_arr[1])
		printf("env: ‘%s’: No such file or directory\n", n->cmd_args_arr[1]);
	if (is_builtin(n->value))
		execute_builtin(n, n->value, 1);
	else if (execve(n->exec_cmd_path, n->cmd_args_arr, n->ms->env_arr) < 0)
		ft_error(errno, ft_strdup(n->cmd_args_arr[0]), n, 1);
}

void	setup_exec(t_exec *e, t_tree_node *n, int pipe_ct)
{
	t_redir	*redir_ptr;

	e->pipe_index = 0;
	e->status = 0;
	while (n->type != END)
	{
		redir_ptr = n->redir;
		if (redir_ptr)
			while (redir_ptr->bwd)
				redir_ptr = redir_ptr->bwd;
		n->redir = redir_ptr;
		n->pipe_ct = pipe_ct;
		init_infiles_outfiles(n->redir, n, &e->status);
		traverse_tree(&n);
	}
}


void prepare_exec(t_tree_node *n, int pipe_ct, t_exec *e)
{
	setup_exec(e, n, pipe_ct);
	while (n->type != END)
	{
		if (!pipe_ct && is_builtin(n->value))
			execute_builtin(n, n->value, 0);
		else if (n->type != PIPE)
		{
			while (n->type == PIPE || n->type == AND || n->type == OR)
				traverse_tree(&n);
			if (n->type != END)
			{
				e->pid = fork();
				if (e->pid < 0)
					ft_error(errno, ft_strdup("fork"), n, 1);
				if (!e->pid) // Child process
				{
					signal(SIGINT, SIG_DFL);
					signal(SIGQUIT, SIG_DFL);
					execute(n, e->pipe_index, pipe_ct);
				}
				else // Parent process ignores Ctrl+C
					signal(SIGINT, SIG_IGN);
				e->pipe_index++;
			}
		}
		traverse_tree(&n);
	}
	close_fds(n, pipe_ct);
}

void finalize_exec(t_tree_node *n, t_exec *e)
{
	while (e->pipe_index-- > 0)
	{
		waitpid(-1, &e->status, 0);
		if (WIFSIGNALED(e->status) && WTERMSIG(e->status) == SIGINT)
			printf("\n");
	}
	signal(SIGINT, SIG_IGN); // Ignore SIGINT after child processes finish
	if (e->status == 256)
		e->status = 127;
	update_exit_status(n->ms->env, e->status);
}

void init_exec(t_tree_node *n, int pipe_ct)
{
	t_exec e;

	prepare_exec(n, pipe_ct, &e);  // Handles setup, fork, and execute
	finalize_exec(n, &e); // Handles waiting and signal handling
}
