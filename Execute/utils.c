/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/08/23 20:26:14 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tree_node	*start_node(t_tree_node *n)
{
	while (n->left)
		n = n->left;
	return (n);
}

int is_empty(char *av)
{
	int i;
	size_t sp_ct;

	i = -1;
	sp_ct = 0;
	while (av[++i])
	{
		if (av[i] == ' ')
			sp_ct++;
	}
	if (sp_ct == ft_strlen(av))
		return (0);
	return (1);
}

void check_infile_cmdpaths(t_tree_node *n)
{
	char	*err_msg;

	while (!n->is_last_node)
	{
		if (n->in_fd < 0)
		{
			err_msg = ft_strjoin("command not found", p->cmd_args[i][0]);
			ft_error(errno, err_msg, p, exit_switch);
		}
		if (!ft_strncmp(p->exec_cmd_path[i], "invalid", 7)
			&& ft_strlen(av[i + 2 + p->hd_shift]))
		{
			err_msg = ft_strjoin("command not found", p->cmd_args[i][0]);
			ft_error(errno, err_msg, p, exit_switch);
		}
		else if (!ft_strlen(av[i + 2 + p->hd_shift]))
		{
			close_fds(p);
			ft_error(errno, ft_strdup("permission denied:"), p, exit_switch);
		}
	}
}

void setup_p_cp_arr(t_exec *p)
{
	int i;

	i = -1;
	p->fd = ft_calloc(p->pipe_ct, sizeof(int *));
	if (!p->fd)
		ft_error(errno, ft_strdup("fd calloc"), p, 1);
	while (++i < p->pipe_ct)
	{
		p->fd[i] = ft_calloc(2, sizeof(int));
		if (!p->fd)
			ft_error(errno, ft_strdup("fd calloc"), p, 1);
	}
	p->pid = ft_calloc(p->cmd_ct, sizeof(int));
	if (!p->pid)
		ft_error(errno, ft_strdup("pid calloc"), p, 1);
}

void close_fds(t_exec *p)
{
	p->j = -1;
	p->k = -1;
	if (p->pipe_ct)
	{
		while (++p->j < p->pipe_ct)
		{
			while (++p->k < 2)
				close(p->fd[p->j][p->k]);
			p->k = -1;
		}
	}
	if (p->outfile > -1)
		close(p->outfile);
	if (p->empty_fd == p->infile && p->empty_fd)
		close(p->empty_fd);
	else
	{
		if (p->empty_fd > -1)
			close(p->empty_fd);
		if (p->infile > -1)
			close(p->infile);
	}
	unlink("tmp.txt");
	unlink("empty.txt");
}

void ft_error(int error, char *str, t_tree_node *n, int exit_switch)
{
	if (!error)
		ft_printf("%s\n", str);
	else
		ft_printf("%s: %s\n", strerror(error), str);
	free(str);
	if (exit_switch)
	{
		free_all(n);
		exit(EXIT_FAILURE);
	}
}
