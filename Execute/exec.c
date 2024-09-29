/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingmohamedalsherif <kingmohamedalsherif@s +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:17:25 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 18:14:09 by kingmohamedalshe ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void execute(t_tree_node *n, int pipe_index, int pipe_ct)
{
	if (n->redir)
	{
		last_redir_fd(n->redir, 'i', &n->use_in_fd);
		last_redir_fd(n->redir, 'o', &n->use_out_fd);
	}
	if (n->use_in_fd < 2 && pipe_index)
		n->use_in_fd = n->pipefd[pipe_index - 1][0];
	if (n->use_out_fd < 2 && pipe_ct && !n->right)
		n->use_out_fd = n->pipefd[pipe_index][1];
	if (dup2(n->use_in_fd, STDIN_FILENO) < 0)
		ft_error(errno, ft_strdup("dup infile"), n, 1);
	if (dup2(n->use_out_fd, STDOUT_FILENO) < 0)
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

void	prepare_exec(t_tree_node *n, int pipe_ct, t_exec *e)
{
	while (n->type == PIPE)
		traverse_tree(&n);
	if (n->type != END)
	{
		e->pid = fork();
		if (e->pid < 0)
			ft_error(errno, ft_strdup("fork"), n, 1);
		if (!e->pid)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			execute(n, e->pipe_index, pipe_ct);
		}
		else
			signal(SIGINT, SIG_IGN);
		e->pipe_index++;
	}
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

void	init_exec(t_tree_node *n, int pipe_ct)
{
	t_exec	e;
	
	setup_exec(&e, n, pipe_ct);
	while (n->type != END)
	{
		if (!pipe_ct && is_builtin(n->value))
			execute_builtin(n, n->value, 0);
		else if (n->type != PIPE)
			prepare_exec(n, pipe_ct, &e);
		traverse_tree(&n);
	}
	close_fds(n, pipe_ct);
	while (e.pipe_index-- > 0)
	{
		waitpid(-1, &e.status, 0);
		if (WIFSIGNALED(e.status) && WTERMSIG(e.status) == SIGINT)
			printf("\n");
	}
	signal(SIGINT, SIG_IGN);
	if (e.status == 256)
		e.status = 127;
	update_exit_status(n->ms->env, e.status);
}
