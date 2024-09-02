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
	int	use_in_fd;
	int	use_out_fd;

	// printf("%s\n", n->token->exec_cmd_path);
	// printf("%s\n", n->token->cmd_args[0]);
	if (n->infile)
		use_in_fd = n->in_fd;
	else if (n->is_first_node)
		use_in_fd = 0;
	else
		use_in_fd = n->pipefd[0];
	if (n->outfile)
		use_out_fd = n->out_fd;
	else if (n->right && n->right->is_last_node)
		use_out_fd = 1;
	else
		use_out_fd = n->pipefd[1];
	// printf("in:%d out:%d\n", use_in_fd, use_out_fd);
	if (dup2(use_in_fd, STDIN_FILENO) < 0)
		ft_error(errno, ft_strdup("dup infile"), n, 1);
	if (dup2(use_out_fd, STDOUT_FILENO) < 0)
		ft_error(errno, ft_strdup("dup outfile"), n, 1);
	close_fds(n);
	if (execve(n->token->exec_cmd_path, n->token->cmd_args, n->p->env) < 0)
		ft_error(errno, ft_strdup("execve"), n, 1);
}

void init_exec(t_tree_node *n)
{
	t_tree_node *n_head;
	int			pid;

	n_head = n;
	while (!n->is_last_node)
	{
		if (n->token->type == CD)
			cd(n);
		if (n->token->type == CMD)
		{
			// printf("%s\n", n->token->exec_cmd_path);
			// printf("%s\n", n->token->cmd_args[0]);
			pid = fork();
			if (pid < 0)
				ft_error(errno, ft_strdup("fork"), n, 1);
			if (!pid)
				execute(n);
			waitpid(pid, NULL, WNOHANG);
		}
		traverse_tree(&n);
	}
	close_fds(n_head);
}
