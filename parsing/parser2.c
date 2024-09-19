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

void	find_exec_path(t_tree_node *n, t_paths p, int **pipefd)
{
	int	i;

	while (n->token->type != END)
	{
		i = -1;
		if (n->token->type == WORD)
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
			n->pipefd = pipefd;
		}
		traverse_tree(&n, 1);
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
	if (!p->split_filepaths)
		exit (1);
	p->env = ms_env_arr;
}

int	**create_pipe_arr(t_tree_node *n, int *pipe_ct)
{
	int	**pipefd;
	int	i;
	int	j;

	*pipe_ct = 0;
	while (n->parent)
	{
		if (n->token->type == PIPE)
			(*pipe_ct)++;
		n = n->parent;
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

void	pipes_n_exec_path(t_tree_node *head, t_ms_var *ms, int *pipe_ct)
{
	t_paths 	p;
	int			**pipefd;

	pipefd = create_pipe_arr(head, pipe_ct);
	init_filepaths(&p, ms->env, ms->env_arr);
	find_exec_path(head, p, pipefd);
}
