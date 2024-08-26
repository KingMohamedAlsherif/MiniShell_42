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

void execute(t_tree_node *n)
{
	if (dup2(n->in_fd, STDIN_FILENO) < 0)
		ft_error(errno, ft_strdup("dup"), n, 1);
	if (dup2(n->out_fd, STDOUT_FILENO) < 0)
		ft_error(errno, ft_strdup("dup"), n, 1);
	close_fds(n);
	if (execve(n->token->exec_cmd_path, n->token->cmd_args, n->p->env) < 0)
		ft_error(errno, ft_strdup("execve"), n, 1);
}

void multiple_cmds(t_tree_node *n)
{
	int	pid;

	while (!n->is_last_node)
	{
		pid = fork();
		if (pid < 0)
			ft_error(errno, ft_strdup("fork"), n, 1);
		if (!pid)
		{
			if (!n->infile)
				n->in_fd = n->pipefd[0];
			if (!n->outfile)
				n->out_fd = n->pipefd[1];
			execute(n);
		}
		while (n->token->type != CMD)
			traverse_tree(&n);
	}
	waitpid(pid, NULL, WNOHANG);
	close_fds(n);
}

void init_exec(t_tree_node *n)
{
	// if (p->infile < 0 || valid_cmds_start)
	// 	p->infile = p->empty_fd;
	if (n->is_last_node)
		execute(n);
	else
		multiple_cmds(n);
}
