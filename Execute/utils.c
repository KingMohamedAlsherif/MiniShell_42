/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/09/02 15:00:44 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tree_node	*start_node(t_tree_node *n)
{
	while (n->parent)
		n = n->parent;
	while (n->left)
		n = n->left;
	return (n);
}

// void	reset_read_flag(t_tree_node **n)
// {
// 	(*n)->is_read = 0;
// 	if ((*n)->parent && ((!(*n)->left && !(*n)->right)
// 		|| ((*n)->left && !(*n)->left->is_read && (*n)->right && !(*n)->right->is_read)))
// 		*n = (*n)->parent;
// 	else if ((*n)->left && (*n)->left->is_read)
// 		*n = (*n)->left;
// 	else
// 		*n = (*n)->right;
// }

void	traverse_tree(t_tree_node **n)
{
	// (*n)->is_read = 1;
	bool	unread_flag;
	bool	read_flag;

	unread_flag = (*n)->is_read;
	read_flag = ((*n)->is_read + 1) % 2;
	(*n)->is_read = read_flag;
	if ((*n)->is_last_node)
		return ;
	if ((*n)->parent && ((!(*n)->left && !(*n)->right)
		|| ((*n)->left && (*n)->left->is_read == read_flag
			&& (*n)->right && (*n)->right->is_read == read_flag)))
		*n = (*n)->parent;
	else if ((*n)->left && (*n)->left->is_read == unread_flag)
		*n = (*n)->left;
	else
		*n = (*n)->right;
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

// void check_infile_cmdpaths(t_tree_node *n)
// {
// 	char	*err_msg;

// 	while (!n->is_last_node)
// 	{
// 		if (n->in_fd < 0)
// 		{
// 			err_msg = ft_strjoin("command not found", p->cmd_args[i][0]);
// 			ft_error(errno, err_msg, p, exit_switch);
// 		}
// 		if (!ft_strncmp(p->exec_cmd_path[i], "invalid", 7)
// 			&& ft_strlen(av[i + 2 + p->hd_shift]))
// 		{
// 			err_msg = ft_strjoin("command not found", p->cmd_args[i][0]);
// 			ft_error(errno, err_msg, p, exit_switch);
// 		}
// 		else if (!ft_strlen(av[i + 2 + p->hd_shift]))
// 		{
// 			close_fds(p);
// 			ft_error(errno, ft_strdup("permission denied:"), p, exit_switch);
// 		}
// 	}
// }

// void setup_p_cp_arr(t_exec *p)
// {
// 	int i;

// 	i = -1;
// 	p->fd = ft_calloc(p->pipe_ct, sizeof(int *));
// 	if (!p->fd)
// 		ft_error(errno, ft_strdup("fd calloc"), p, 1);
// 	while (++i < p->pipe_ct)
// 	{
// 		p->fd[i] = ft_calloc(2, sizeof(int));
// 		if (!p->fd)
// 			ft_error(errno, ft_strdup("fd calloc"), p, 1);
// 	}
// 	p->pid = ft_calloc(p->cmd_ct, sizeof(int));
// 	if (!p->pid)
// 		ft_error(errno, ft_strdup("pid calloc"), p, 1);
// }

void close_fds(t_tree_node *n, int pipe_ct)
{
	t_tree_node	*n_0;
	int			i;

	n_0 = start_node(n);
	// printf("%s: %d %d\n", n_0->token->cmd_args[0], n_0->pipefd[0], n_0->pipefd[1]);
	i = -1;
	while (++i < pipe_ct)
	{
		close(n_0->pipefd[i][0]);
		close(n_0->pipefd[i][1]);
	}
	while (!n_0->is_last_node)
	{
		if (n_0->in_fd > 2)
			close(n_0->in_fd);
		if (n_0->out_fd > 2)
			close(n_0->out_fd);
		traverse_tree(&n_0);
	}
	unlink("tmp.txt");
	unlink("empty.txt");
}

void ft_error(int error, char *str, t_tree_node *n, int exit_switch)
{
	if (!error)
		ft_printf("%s\n", str);
	else
		ft_printf("%s: %s\n", str, strerror(error));
	free(str);
	(void)n;
	if (exit_switch)
	{
		// free_all(n);
		exit(EXIT_FAILURE);
	}
}
