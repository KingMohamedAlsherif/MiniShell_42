/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:09:22 by chon              #+#    #+#             */
/*   Updated: 2024/09/13 19:34:14 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	last_redir_fd(t_redir *redir, char type)
{
	while (redir->fwd)
		redir = redir->fwd;
	if (type == 'i')
	{
		while (redir->bwd
		&& (!redir->in_fd || !redir->heredoc_delim))
			redir = redir->bwd;
		return (redir->in_fd);
	}
	else
	{
		while (redir->bwd
		&& (!redir->out_fd || !redir->is_append))
			redir = redir->bwd;
		return (redir->out_fd);
	}
}

t_tree_node	*start_node(t_tree_node *n)
{
	while (n->parent)
		n = n->parent;
	while (n->left)
		n = n->left;
	return (n);
}

void	traverse_tree(t_tree_node **n, int read_flag)
{
	bool	unread_flag;

	unread_flag = (read_flag + 1) % 2;
	(*n)->is_read = read_flag;
	if ((*n)->type == END)
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

void	reset_read_flag(t_tree_node **n)
{
	(*n)->is_read = 0;
	while ((*n)->right)
	{
		*n = (*n)->right;
		(*n)->is_read = 0;
	}
	while (!(*n)->is_read)
		*n = (*n)->parent;
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

void	close_redir_fds(t_redir *redir)
{
	while (redir)
	{
		if (redir->in_fd > 2)
			close(redir->in_fd);
		if (redir->out_fd > 2)
			close(redir->out_fd);
		if (redir->heredoc_delim)
			unlink(redir->filename);
		redir = redir->fwd;
	}
}

void	close_fds(t_tree_node *n, int pipe_ct)
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
		close_redir_fds(n_0->redir);
		traverse_tree(&n_0, 0);
	}
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
