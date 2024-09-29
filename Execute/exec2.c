/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingmohamedalsherif <kingmohamedalsherif@s +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:17:25 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 18:14:24 by kingmohamedalshe ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	prepare_exec2(t_tree_node *n, int pipe_ct, t_exec *e)
{
	while (n->type == PIPE || n->type == AND || n->type == OR)
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
			execute1(n, e->pipe_index, pipe_ct);
		}
		else
			signal(SIGINT, SIG_IGN);
		e->pipe_index++;
	}
}

void	prepare_exec(t_tree_node *n, int pipe_ct, t_exec *e)
{
	setup_exec(e, n, pipe_ct);
	while (n->type != END)
	{
		if (!pipe_ct && is_builtin(n->value))
			execute_builtin(n, n->value, 0);
		else if (n->type != PIPE)
			prepare_exec2(n, pipe_ct, e);
		traverse_tree(&n);
	}
	close_fds(n, pipe_ct);
}

void	finalize_exec(t_tree_node *n, t_exec *e)
{
	while (e->pipe_index-- > 0)
	{
		waitpid(-1, &e->status, 0);
		if (WIFSIGNALED(e->status) && WTERMSIG(e->status) == SIGINT)
			printf("\n");
	}
	signal(SIGINT, SIG_IGN);
	if (e->status == 256)
		e->status = 127;
	update_exit_status(n->ms->env, e->status);
}

void	init_exec(t_tree_node *n, int pipe_ct)
{
	t_exec	e;

	prepare_exec(n, pipe_ct, &e);
	finalize_exec(n, &e);
}
