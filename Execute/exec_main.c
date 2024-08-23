/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:26:17 by chon              #+#    #+#             */
/*   Updated: 2024/08/23 19:59:02 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void init_outfile_and_arrays(t_tree_node *n)
{
	int	i;

	if (!n->is_here_doc)
		n->out_fd = open(n->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else
		n->out_fd = open(n->outfile, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (n->out_fd < 0)
		ft_error(errno, ft_strdup(n->outfile), n, 1);
	check_filepaths(p, av);
}

void init_infile(t_tree_node *n)
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
}

void init_filepaths(t_tree_node *n, char **env)
{
	int	i;

	n->filepaths = NULL;
	n->cmd_args = NULL;
	n->all_cmd_paths = NULL;
	n->exec_cmd_path = NULL;
	n->pipefd = NULL;
	// n->pid = NULL;
	// n->pipe_ct = 0;
	n->empty_fd = 0;

	// n->env = env;
}

int main(int ac, char **av, char **env)
{
	t_exec p;
	t_token t;
	t_tree_node n;
	char	*tmp_args[] = { "1", "2", "3" , NULL};

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
		init_infile(&n);
		init_outfile_and_arrays(&n);
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
	return (0);
}
