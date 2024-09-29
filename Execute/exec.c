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

void	execute1(t_tree_node *n, int pipe_index, int pipe_ct)
{
	int	use_in_fd;
	int	use_out_fd;

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
	if (dup2(use_in_fd, STDIN_FILENO) < 0)
		ft_error(errno, ft_strdup("dup infile"), n, 1);
	if (dup2(use_out_fd, STDOUT_FILENO) < 0)
		ft_error(errno, ft_strdup("dup outfile"), n, 1);
	execute2(n, pipe_ct);
}

void	execute2(t_tree_node *n, int pipe_ct)
{
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
