/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:16:55 by chon              #+#    #+#             */
/*   Updated: 2024/09/13 19:38:07 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void init_outfile(t_tree_node *n)
{
	t_redir	*redir_ptr;
	// maybe std_input can replace empty_fd
	// n->empty_fd = open("empty.txt", O_TRUNC | O_CREAT, 0777);
	// if (n->empty_fd < 0)
	// 	ft_error(errno, ft_strdup("empty.txt"), n, 1);

	n->out_fd_node = NULL;
	redir_ptr = n->redir;
	while (redir_ptr->next)
	{
		if (redir_ptr->regular_outfile || redir_ptr->is_append)
		{
			n->out_fd_node = redir_ptr;
			if (redir_ptr->regular_outfile)
				redir_ptr->regular_outfile = open(redir_ptr->filename, O_WRONLY | O_TRUNC | O_CREAT, 0777);
			else
				redir_ptr->regular_outfile = open(redir_ptr->filename, O_WRONLY | O_APPEND | O_CREAT, 0777);
			if (redir_ptr->regular_outfile < 0)
				ft_error(errno, ft_strdup(redir_ptr->filename), n, 1);
		}
		redir_ptr = redir_ptr->next;
	}
	// printf("%d\n", n->out_fd);
}

void init_infile_outfile(t_tree_node *n)
{
	t_redir	*redir_ptr;
	char	*line;

	redir_ptr = n->redir;
	n->in_fd_node = NULL;
	while (redir_ptr->next)
	{
		if (redir_ptr->regular_infile || redir_ptr->is_heredoc)
			n->in_fd_node = redir_ptr;
		redir_ptr = redir_ptr->next;
	}
	if (n->in_fd_node && n->in_fd_node->is_heredoc)
	{
		n->in_fd_node->regular_infile = open("tmp.txt", O_RDWR | O_TRUNC | O_CREAT, 0777);
		if (n->in_fd_node->regular_infile < 0)
			ft_error(errno, ft_strdup("tmp.txt"), n, 1);
		line = get_next_line(0);
		while (ft_strncmp(n->in_fd_node->heredoc_delim, line, ft_strlen(line) - 1))
		{
			write(n->in_fd_node->regular_infile, line, ft_strlen(line));
			free(line);
			line = get_next_line(0);
		}
		free(line);
		close(n->in_fd_node->regular_infile);
		n->in_fd_node->regular_infile = open("tmp.txt", O_RDONLY);
	}
	else if (n->in_fd_node && n->in_fd_node->regular_infile)
	{
		n->in_fd_node->regular_infile = open(n->in_fd_node->filename, O_RDONLY);
		if (n->in_fd_node->regular_infile < 0)
			ft_error(errno, n->in_fd_node->filename, n, 0);
	}
	// printf("%d\n", n->redir->in_fd);
	init_outfile(n);
}

void	fill_files(t_tree_node *n, t_paths p, int **pipefd)
{
	int	i;

	while (n->token->type != END)
	{
		i = -1;
		if (n->token->type == CMD)
		{
			while (p.split_filepaths[++i])
			{
				p.filepath_0 = ft_strjoin(p.split_filepaths[i], "/");
				p.filepath = ft_strjoin(p.filepath_0, n->token->value);
				free(p.filepath_0);
				if (access(p.filepath, X_OK) > -1)
				{
					n->token->exec_cmd_path = ft_strdup(p.filepath);
					free(p.filepath);
					break;
				}
				free(p.filepath);
			}
			init_infile_outfile(n);
			n->pipefd = pipefd;
		}
		traverse_tree(&n);
	}
}

void init_filepaths(t_paths *p, t_lst *ms_env, char **ms_env_arr)
{
	p->split_filepaths = NULL;
	p->filepath_0 = NULL;
	p->filepath = NULL;
	while (ms_env)
	{
		if (!ft_strncmp(ms_env->var, "PATH", 5))
			break ;
		ms_env = ms_env->fwd;
	}
	p->split_filepaths = ft_split(ms_env->val, ':');
	// if (!p->split_filepaths)
	// 	ft_error(errno, ft_strdup("split_filepaths"), p, 1);
	p->env = ms_env_arr;
}

int	**create_pipe_arr(t_tree_node *n, int *pipe_ct)
{
	int	**pipefd;
	int	i;
	int	j;

	*pipe_ct = 0;
	while (n->left)
	{
		if (n->token->type == PIPE)
			(*pipe_ct)++;
		n = n->left;
	}
	pipefd = malloc(sizeof(int *) * *pipe_ct);
	if (!pipefd)
		ft_error(errno, ft_strdup("pipe malloc"), n, 1);
	i = 0;
	j = *pipe_ct;
	while (--j > -1)
	{
		pipefd[i] = malloc(sizeof(int) * 2);
		if (pipe(pipefd[i++]) < 0)
			ft_error(errno, ft_strdup("pipe"), n, 1);
	}
	return (pipefd);
}

void	create_fds(t_tree_node **ast, t_ms_var *ms)
{
	t_tree_node	*ast_head;
	t_paths 	p;
	int			**pipefd;
	int			pipe_ct;

	ast_head = *ast;
	pipefd = create_pipe_arr(ast_head, &pipe_ct);
	init_filepaths(&p, ms->env, ms->env_arr);
	fill_files(ast_head, p, pipefd);
	// check_filepaths(start_node(ast_head));
	// printf("%s\n", n->token->str);
	// printf("%d\n", n->token->type);
	// printf("infile:%d\n", n->in_fd);
	// printf("%s\n", n->token->exec_cmd_path);
	// printf("%s\n", n->token->cmd_args_arr[3]);
	// printf("%d\n", n->is_last_node);
	// printf("%s: %d %d\n", n->parent->right->token->cmd_args_arr[0], n->pipefd[0], n->pipefd[1]);
	init_exec(ast_head, pipe_ct);
	// free_all(start_node(ast_head));
	// return (ast_head);
}
