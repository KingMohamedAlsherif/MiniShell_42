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

void	finalize_exec_cmd_path(char	**exec_cmd_path, char *value)
{
	if (is_number(value))
		*exec_cmd_path = ft_strdup("?");
	else if (!*exec_cmd_path)
		*exec_cmd_path = ft_strdup("invalid");
}

void	create_cmd_args_arr(t_tree_node *n)
{
	int		str_ct;
	t_args	*args_ptr;
	int		i;

	finalize_exec_cmd_path(&n->exec_cmd_path, n->value);
	str_ct = 0;
	args_ptr = n->cmd_args;
	while (args_ptr)
	{
		str_ct++;
		args_ptr = args_ptr->next;
	}
	n->cmd_args_arr = malloc(sizeof(char *) * (str_ct + 1));
	if (!n->cmd_args_arr)
		return ;
	i = -1;
	while (++i < str_ct)
	{
		n->cmd_args_arr[i] = ft_strdup(n->cmd_args->arg);
		args_ptr = n->cmd_args;
		n->cmd_args = n->cmd_args->next;
		free(args_ptr->arg);
		free(args_ptr);
	}
	n->cmd_args_arr[i] = NULL;
}

void	exec_path_args_arr(t_tree_node *n, t_paths p, int **pipefd)
{
	int	i;

	while (n->type != END)
	{
		i = -1;
		if (n->type == WORD)
		{
			while (p.split_filepaths[++i])
			{
				p.filepath_0 = ft_strjoin(p.split_filepaths[i], "/", 0, 0);
				p.filepath = ft_strjoin(p.filepath_0, n->value, 1, 0);
				if (access(p.filepath, X_OK) > -1)
				{
					n->exec_cmd_path = ft_strdup(p.filepath);
					free(p.filepath);
					break ;
				}
				free(p.filepath);
			}
			n->pipefd = pipefd;
			create_cmd_args_arr(n);
		}
		traverse_tree(&n);
	}
	free_char_arr(p.split_filepaths, NULL);
}

void init_filepaths(t_paths *p, t_lst *ms_env)
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
	if (!p->split_filepaths)
		return ;
}

void	pipes_n_exec_path(t_tree_node *head, t_ms_var *ms, int *pipe_ct)
{
	t_paths	p;
	int		**pipefd;
	int		i;
	int		j;

	*pipe_ct = 0;
	while (head->parent)
	{
		if (head->parent->type == PIPE)
			(*pipe_ct)++;
		head = head->parent;
	}
	pipefd = malloc(sizeof(int *) * *pipe_ct);
	if (!pipefd)
		ft_error(errno, ft_strdup("pipe malloc"), start_node(head), 1);
	i = 0;
	j = *pipe_ct;
	while (--j > -1)
	{
		pipefd[i] = malloc(sizeof(int) * 2);
		if (pipe(pipefd[i++]) < 0)
			ft_error(errno, ft_strdup("pipe"), head, 1);
	}
	init_filepaths(&p, ms->env);
	exec_path_args_arr(start_node(head), p, pipefd);
}
