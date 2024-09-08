/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:16:55 by chon              #+#    #+#             */
/*   Updated: 2024/09/02 16:07:56 by chon             ###   ########.fr       */
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

void	create_tokens_tree(t_tree_node **n_head, t_token **t_head, t_paths p)
{
	t_token		*t_1;
	t_token		*t_2;
	t_token		*t_3;
	t_token		*t_4;
	t_tree_node	*n_1;
	t_tree_node	*n_2;
	t_tree_node	*n_3;
	t_tree_node	*n_4;
	t_tree_node	*n_5;
	t_tree_node	*n_6;
	char		**args_cat;
	char		**args_ls;
	char		**args_grep;
	char		**args_grep_2;
	char		**args_echo;
	char		**args_cd;
	char		**args_sleep;

	t_1 = malloc(sizeof(t_token));
	*t_head = t_1;
	t_2 = malloc(sizeof(t_token));
	t_3 = malloc(sizeof(t_token));
	t_4 = malloc(sizeof(t_token));
	n_1 = malloc(sizeof(t_tree_node));
	*n_head = n_1;
	n_2 = malloc(sizeof(t_tree_node));
	n_3 = malloc(sizeof(t_tree_node));
	n_4 = malloc(sizeof(t_tree_node));
	n_5 = malloc(sizeof(t_tree_node));
	n_6 = malloc(sizeof(t_tree_node));
	
	args_cat = malloc(sizeof(char *) * 2);
	args_cat[0] = "cat";
	args_cat[1] = NULL;
	args_ls = malloc(sizeof(char *) * 2);
	args_ls[0] = "ls";
	args_ls[1] = NULL;
	args_grep = malloc(sizeof(char *) * 3);
	args_grep[0] = "grep";
	args_grep[1] = "x";
	args_grep[2] = NULL;
	args_grep_2 = malloc(sizeof(char *) * 3);
	args_grep_2[0] = "grep";
	args_grep_2[1] = "x";
	args_grep_2[2] = NULL;
	args_echo = malloc(sizeof(char *) * 4);
	args_echo[0] = "echo";
	args_echo[1] = "-n";
	args_echo[2] = "hello";
	args_echo[3] = NULL;
	args_cd = malloc(sizeof(char *) * 2);
	args_cd[0] = "..";
	args_cd[1] = NULL;
	args_sleep = malloc(sizeof(char *) * 2);
	args_sleep[0] = "30";
	args_sleep[1] = NULL;

// 1 cmd

	// t_1->type = CMD;
	// // t_1->type = CD;
	// t_1->str = "ls";
	// // t_1->str = "cat";
	// // t_1->str = "echo";
	// // t_1->str = "cd";
	// t_1->cmd_args_arr = args_ls;
	// // t_1->cmd_args_arr = args_cat;
	// // t_1->cmd_args_arr = args_echo;
	// t_1->cmd_args_arr = args_cd;
	// t_1->next = t_2;
	// n_1->token = t_1;
	// init_tree_node(n_1, p);
	// // n_1->infile = "1";
	// // n_1->outfile = "outttt";

	// n_1->right = n_2;
	// t_2->type = END;
	// t_2->next = NULL;
	// n_2->is_last_node = 1;
	// n_2->parent = *n_head;

// 2 cmds

	// t_1->type = PIPE;
	// t_1->str = "null";
	// n_1->token = t_1;
	// init_tree_node(n_1, p);
	
	// // t_1->next = t_2;
	// // t_2->type = CMD;
	// // t_2->str = "grep";
	// // t_2->cmd_args_arr = args_grep;
	// // n_1->left = n_2;
	// // n_2->token = t_2;
	// // init_tree_node(n_2, p);
	// // n_2->is_first_node = 1;
	// // n_2->infile = "1";
	// // n_2->parent = *n_head;
	
	// // t_2->next = t_3;
	// // t_3->type = CMD;
	// // t_3->str = "cat";
	// // t_3->cmd_args_arr = args_cat;
	// // t_3->next = NULL;
	// // n_1->right = n_3;
	// // n_3->token = t_3;
	// // init_tree_node(n_3, p);
	// // // n_3->outfile = "out";
	// // n_3->parent = *n_head;

	// t_1->next = t_2;
	// t_2->type = CD;
	// t_2->str = "cd";
	// t_2->cmd_args_arr = args_cd;
	// n_1->left = n_2;
	// n_2->token = t_2;
	// init_tree_node(n_2, p);
	// n_2->is_first_node = 1;
	// // n_2->infile = "1";
	// n_2->parent = *n_head;
	
	// t_2->next = t_3;
	// t_3->type = CMD;
	// t_3->str = "ls";
	// t_3->cmd_args_arr = args_ls;
	// t_3->next = NULL;
	// n_1->right = n_3;
	// n_3->token = t_3;
	// init_tree_node(n_3, p);
	// // n_3->outfile = "out";
	// n_3->parent = *n_head;

	// n_3->right = n_4;
	// n_4->token = NULL;
	// init_tree_node(n_4, p);
	// n_4->is_last_node = 1;
	// n_4->parent = n_3;

// 3 cmds

	t_1->type = PIPE;
	t_1->str = "null";
	t_1->exec_cmd_path = NULL;
	n_1->token = t_1;
	init_tree_node(n_1, p);
	
	// t_1->next = t_2;
	// t_2->type = CD;
	// t_2->str = "cd";
	// t_2->cmd_args_arr = args_cd;
	// n_1->left = n_2;
	// n_2->token = t_2;
	// init_tree_node(n_2, p);
	// n_2->is_first_node = 1;
	// // n_2->infile = "1";
	// n_2->parent = *n_head;

	t_1->next = t_2;
	t_2->type = CMD;
	t_2->str = "sleep";
	t_2->cmd_args_arr = args_sleep;
	n_1->left = n_2;
	n_2->token = t_2;
	init_tree_node(n_2, p);
	n_2->is_first_node = 1;
	// n_2->infile = "1";
	n_2->parent = *n_head;

	// t_1->next = t_2;
	// t_2->type = CMD;
	// t_2->str = "grep";
	// t_2->cmd_args_arr = args_grep;
	// n_1->left = n_2;
	// n_2->token = t_2;
	// init_tree_node(n_2, p);
	// n_2->is_first_node = 1;
	// n_2->infile = "1";
	// n_2->parent = *n_head;

	// t_1->next = t_2;
	// t_2->type = CMD;
	// t_2->str = "ls";
	// t_2->cmd_args_arr = args_ls;
	// n_1->left = n_2;
	// n_2->token = t_2;
	// init_tree_node(n_2, p);
	// n_2->is_first_node = 1;
	// // n_2->infile = "1";
	// n_2->parent = *n_head;
	
	// t_2->next = t_3;
	// t_3->type = CMD;
	// t_3->str = "grep";
	// t_3->cmd_args_arr = args_grep_2;
	// n_1->right = n_3;
	// n_3->token = t_3;
	// init_tree_node(n_3, p);
	// n_3->parent = *n_head;

	t_2->next = t_3;
	t_3->type = CMD;
	t_3->str = "ls";
	t_3->cmd_args_arr = args_ls;
	n_1->right = n_3;
	n_3->token = t_3;
	init_tree_node(n_3, p);
	n_3->parent = *n_head;

	n_3->right = n_4;
	n_4->token = t_1;
	init_tree_node(n_4, p);
	n_4->parent = n_3;

	t_3->next = t_4;
	t_4->type = CMD;
	t_4->str = "cat";
	t_4->cmd_args_arr = args_cat;
	t_4->next = NULL;
	n_4->right = n_5;
	n_5->token = t_4;
	init_tree_node(n_5, p);
	// n_5->outfile = "new";
	n_5->parent = n_4;

	n_5->right = n_6;
	n_6->token = NULL;
	init_tree_node(n_6, p);
	n_6->is_last_node = 1;
	n_6->parent = n_5;
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
int	parse(int ac, char **av, char **env)
{
	t_token 	*t_head;
	t_tree_node	*n_head;
	t_tree_node	*n;
	t_paths 	p;
	int			**pipefd;
	int			pipe_ct;
	int			cmd_ct;

	(void)ac;
	(void)av;
	n_head = NULL;
	t_head = NULL;
	// cmd_ct = count_cmds(t_head);
	pipe_ct = 2;
	cmd_ct = 3;
	pipefd = create_pipe_arr(pipe_ct, n_head);
	if (env)
	{
		init_filepaths(&p, env);
		create_tokens_tree(&n_head, &t_head, p);
		get_exec_cmd_paths(p, t_head);
		n = start_node(n_head);
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
		init_exec(n, pipe_ct, cmd_ct);
		// free_all(start_node(n_head));
	}
	// return (n_head);
	return (0);
}