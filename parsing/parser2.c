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
	// maybe std_input can replace empty_fd
	// n->empty_fd = open("empty.txt", O_TRUNC | O_CREAT, 0777);
	// if (n->empty_fd < 0)
	// 	ft_error(errno, ft_strdup("empty.txt"), n, 1);
	if (n->outfile)
	{
		if (!n->is_here_doc)
			n->out_fd = open(n->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
		else
			n->out_fd = open(n->outfile, O_WRONLY | O_APPEND | O_CREAT, 0777);
		if (n->out_fd < 0)
			ft_error(errno, ft_strdup(n->outfile), n, 1);
	}
	// printf("%d\n", n->out_fd);
}

void init_infile_outfile(t_tree_node *n)
{
	char *line;

	// if (n->is_here_doc || (!n->infile && n->is_first_node))
	if (n->is_here_doc)
	{
		n->in_fd = open("tmp.txt", O_RDWR | O_TRUNC | O_CREAT, 0777);
		if (n->in_fd < 0)
			ft_error(errno, ft_strdup("tmp.txt"), n, 1);
		line = get_next_line(0);
		while (ft_strncmp(n->delimiter, line, ft_strlen(line) - 1))
		{
			write(n->in_fd, line, ft_strlen(line));
			free(line);
			line = get_next_line(0);
		}
		free(line);
		close(n->in_fd);
		n->in_fd = open("tmp.txt", O_RDONLY);
	}
	else if (n->infile)
	{
		n->in_fd = open(n->infile, O_RDONLY);
		if (n->in_fd < 0)
			ft_error(errno, n->infile, n, 0);
	}
	// printf("%d\n", n->in_fd);
	init_outfile(n);
}

void	get_exec_cmd_paths(t_paths p, t_token *t)
{
	int	i;

	while (t)
	{
		i = -1;
		if (t->type == CMD)
		{
			while (p.split_filepaths[++i])
			{
				p.filepath_0 = ft_strjoin(p.split_filepaths[i], "/");
				p.filepath = ft_strjoin(p.filepath_0, t->value);
				free(p.filepath_0);
				if (access(p.filepath, X_OK) > -1)
				{
					t->exec_cmd_path = ft_strdup(p.filepath);
					free(p.filepath);
					break;
				}
				free(p.filepath);
			}
			if (!p.split_filepaths[i])
				t->exec_cmd_path = ft_strdup("invalid");
		}
		t = t->next;
	}
}

void init_filepaths(t_paths *p, char **env)
{
	p->all_filepaths = NULL;
	p->split_filepaths = NULL;
	p->filepath_0 = NULL;
	p->filepath = NULL;
	p->all_filepaths = getenv("PATH");
    // if (!p->all_filepaths)
	// 	ft_error(errno, ft_strdup("env"), NULL, 1);
	p->split_filepaths = ft_split(p->all_filepaths, ':');
	// if (!p->split_filepaths)
	// 	ft_error(errno, ft_strdup("split_filepaths"), p, 1);
	p->env = env;
}

// int	count_cmds(t_token *t_head)
// {
// 	int	cmd_ct;

// 	cmd_ct = 0;
// 	while (t_head->next)
// 	{
// 		if (t_head->type == CMD)
// 			cmd_ct++;	
// 	}
// 	return (cmd_ct);
// }

void	init_tree_node(t_tree_node *n, t_paths p)
{
	n->p = &p;
	n->in_fd = 0;
	n->out_fd = 1;
	n->infile = NULL;
	n->outfile = NULL;
	n->is_here_doc = 0;
	n->delimiter = NULL;
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	n->is_first_node = 0;
	n->is_last_node = 0;
	n->is_read = 0;
}

int	**create_pipe_arr(int pipe_ct, t_tree_node *n)
{
	int	**pipefd;
	int	i;

	pipefd = malloc(sizeof(int *) * pipe_ct);
	if (!pipefd)
		ft_error(errno, ft_strdup("pipe malloc"), n, 1);
	i = 0;
	while (--pipe_ct > -1)
	{
		pipefd[i] = malloc(sizeof(int) * 2);
		if (pipe(pipefd[i++]) < 0)
			ft_error(errno, ft_strdup("pipe"), n, 1);
	}
	return (pipefd);
}

// t_tree_node	*parse(t_token *t_head, char **env)
void	create_fds(t_tree_node **ast, t_lst *ms_env)
{
	t_token 	*t_head;
	t_tree_node	*n_head;
	t_tree_node	*n;
	t_paths 	p;
	int			**pipefd;
	int			pipe_ct;

	n_head = NULL;
	t_head = NULL;
	n = *ast;
	pipe_ct = 0;
	while (n->left)
	{
		if (n->token->type == PIPE)
			pipe_ct++;
		n = n->left;
	}
	pipefd = create_pipe_arr(pipe_ct, n_head);
	init_filepaths(&p, ms_env);
	create_tokens_tree(&n_head, &t_head, p);
	get_exec_cmd_paths(p, t_head);
	while (!n->is_last_node)
	{
		init_infile_outfile(n);
		n->pipefd = pipefd;
		traverse_tree(&n);
	}
	// check_filepaths(start_node(n_head));
	n = start_node(n_head);
	// printf("%s\n", n->token->str);
	// printf("%d\n", n->token->type);
	// printf("infile:%d\n", n->in_fd);
	// printf("%s\n", n->token->exec_cmd_path);
	// printf("%s\n", n->token->cmd_args_arr[3]);
	// printf("%d\n", n->is_last_node);
	// printf("%s: %d %d\n", n->parent->right->token->cmd_args_arr[0], n->pipefd[0], n->pipefd[1]);
	init_exec(n, pipe_ct);
	// free_all(start_node(n_head));
	// return (n_head);
	return (0);
}