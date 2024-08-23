/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:16:55 by chon              #+#    #+#             */
/*   Updated: 2024/08/23 20:24:56 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_exec_cmd_paths(t_paths p, t_token *t, char **env)
{
	int	i;

	while (t)
	{
		i = -1;
		if (t->type = CMD)
		{
			while (p.split_filepaths[++i])
			{
				p.filepath_0 = ft_strjoin(p.split_filepaths[i], "/");
				p.filepath = ft_strjoin(p.filepath_0, t->str);
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

void init_outfile(t_tree_node *n)
{
	if (!n->is_here_doc)
		n->out_fd = open(n->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else
		n->out_fd = open(n->outfile, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (n->out_fd < 0)
		ft_error(errno, ft_strdup(n->outfile), n, 1);
}

void init_infile_outfile(t_tree_node *n)
{
	char *line;

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
	else
	{
		n->in_fd = open(n->infile, O_RDONLY);
		if (n->in_fd < 0)
			ft_error(errno, n->infile, n, 0);
	}
	init_outfile(n);
}

void init_filepaths(t_paths *p, char **env)
{
	int	i;

	p->all_filepaths = NULL;
	p->split_filepaths = NULL;
	p->filepath_0 = NULL;
	p->filepath = NULL;
	i = 0;
	while (env[i] && !ft_strnstr(env[i], "PATH", 4))
		i++;
	if (env[i])
		p->all_filepaths = ft_substr(env[i], 5, ft_strlen(env[i]) - 5);
	p->split_filepaths = ft_split(p->all_filepaths, ':');
	// if (!p->split_filepaths)
	// 	ft_error(errno, ft_strdup("split_filepaths"), p, 1);
	// p->env = env;
}

// t_tree_node	*parse(t_token *t_head, char **env)
t_tree_node	*parse(int ac, char **av, char **env)
{
	t_token 	*t_head;
	t_tree_node	*n_head;
	t_paths 	p;
	char		*tmp_args[] = { "1", "2", "3" , NULL};
	int			i;

	n.token.type = 0;
	n.token.str = "ls";
	n.cmd_args = tmp_args;
	n.in_fd = 0;
	n.out_fd = -1;
	n.infile = NULL;
	n.outfile = NULL;
	n.is_here_doc = 0;
	n.delimiter = NULL;
	n.left = NULL;
	n.right = NULL;
	if (env)
	{
		init_filepaths(&p, env);
		get_exec_cmd_paths(p, t_head, env);
		init_infile(&n);
		check_filepaths(start_node(n_head));
		n.empty_fd = open("empty.txt", O_TRUNC | O_CREAT, 0777);
		if (n.empty_fd < 0)
			ft_error(errno, ft_strdup("empty.txt"), &p, 1);
		pipex(&p, av[1]);
		free_all(&p);
	}
	else
	{
		ft_printf("Ensure ENV exists and ");
		ft_printf("input either (i) an infile or (ii) here_doc and LIMITER, ");
		ft_printf("at least two cmd args, and an outfile\n");
	}
	return (n_head);
}