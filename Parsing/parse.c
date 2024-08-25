/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:16:55 by chon              #+#    #+#             */
/*   Updated: 2024/08/25 15:14:49 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void init_outfile(t_tree_node *n)
{
	// maybe std_input can replace empty_fd
	n->empty_fd = open("empty.txt", O_TRUNC | O_CREAT, 0777);
	if (n->empty_fd < 0)
		ft_error(errno, ft_strdup("empty.txt"), n, 1);
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
	n->p = p;
	n->in_fd = 0;
	n->out_fd = -1;
	n->infile = NULL;
	n->outfile = NULL;
	n->is_here_doc = 0;
	n->delimiter = NULL;
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	n->is_last_node = 0;
	n->is_read = 0;
}

void	create_tokens_tree(t_tree_node *n_head, t_token *t_head, t_paths p)
{
	t_token		*t;
	t_tree_node	*n;
	char		*ls_args[] = {NULL};

	t = t_head;
	t = malloc(sizeof(t_token) * 3);
	n = n_head;
	n = malloc(sizeof(t_tree_node) * 3);
	
	t->type = PIPE;
	n->token = *t;
	init_tree_node(n, p);
	
	t = t->next;
	t->type = CMD;
	t->str = "ls";
	t->cmd_args = ls_args;
	n = n->left;
	n->token = *t;
	init_tree_node(n, p);
	n->parent = n_head;
	
	t = t->next;
	t->type = CMD;
	t->str = "ls";
	t->cmd_args = ls_args;
	n = n_head->right;
	n->token = *t;
	init_tree_node(n, p);
	n->parent = n_head;
	n->is_last_node = 1;
}

// t_tree_node	*parse(t_token *t_head, char **env)
int	main(int ac, char **av, char **env)
{
	t_token 	*t_head;
	t_tree_node	*n_head;
	t_tree_node	*n;
	t_paths 	p;

	(void)ac;
	(void)av;
	n_head = NULL;
	t_head = NULL;
	create_tokens_tree(n_head, t_head, p);
	// cmd_ct = count_cmds(t_head);
	if (env)
	{
		init_filepaths(&p, env);
		get_exec_cmd_paths(p, t_head);
		n = n_head;
		while (!n->is_last_node)
		{
			init_infile_outfile(n);
			traverse_tree(&n);
		}
		// check_filepaths(start_node(n_head));
		n = n_head;
		while (!n->is_last_node)
		{
			init_exec(n);
			traverse_tree(&n);
		}
		// free_all(&p);
	}
	// return (n_head);
	return (0);
}