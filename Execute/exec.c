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

void execute(t_tree_node *n, int pipe_index, int pipe_ct)
{
	int	use_in_fd;
	int	use_out_fd;

	// printf("%s\n", n->token->exec_cmd_path);
	// printf("%s\n", n->token->cmd_args[0]);
	// printf("%d\n", pipe_index);
	if (n->infile)
		use_in_fd = n->in_fd;
	else if (n->is_first_node || pipe_index - 1 < 0)
		use_in_fd = 0;
	else
		use_in_fd = n->pipefd[pipe_index - 1][0];
	if (n->outfile)
		use_out_fd = n->out_fd;
	else if (n->right && n->right->is_last_node)
		use_out_fd = 1;
	else
		use_out_fd = n->pipefd[pipe_index][1];
	// printf("in:%d out:%d\n", use_in_fd, use_out_fd);
	if (dup2(use_in_fd, STDIN_FILENO) < 0)
		ft_error(errno, ft_strdup("dup infile"), n, 1);
	if (dup2(use_out_fd, STDOUT_FILENO) < 0)
		ft_error(errno, ft_strdup("dup outfile"), n, 1);
	close_fds(n, pipe_ct);
	if (execve(n->token->exec_cmd_path, n->token->cmd_args_arr, n->p->env) < 0)
		ft_error(errno, ft_strdup("execve"), n, 1);
}

void init_exec(t_tree_node *n, int pipe_ct, int cmd_ct)
{
	t_tree_node *n_head;
	int			pid;
	int			i;

	n_head = n;
	i = 0;
	while (!n->is_last_node)
	{
		if ((n->parent && n->parent->token->type == AND
			&& n->token->type == CD) || cmd_ct == 1)
			cd(n);
		if (n->token->type == CMD)
		{
			// printf("%s\n", n->token->exec_cmd_path);
			// printf("%s\n", n->token->cmd_args[0]);
			pid = fork();
			if (pid < 0)
				ft_error(errno, ft_strdup("fork"), n, 1);
			if (!pid)
				execute(n, i, pipe_ct);
			waitpid(pid, NULL, WNOHANG);
			i++;
		}
		traverse_tree(&n);
	}
	close_fds(n_head, pipe_ct);
}
