/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:18:37 by chon              #+#    #+#             */
/*   Updated: 2024/09/06 13:14:09 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_lst_node(t_lst *node)
{
	free(node->var_n_val);
	free(node->var);
	free(node->val);
	free(node);
}

void	free_int_array(int **two_d, int pipe_ct)
{
	int i;

	i = -1;
	if (two_d)
	{
		while (++i < pipe_ct)
			free(two_d[i]);
		free(two_d);
	}
}

void	free_char_arr(char **two_d, char ***three_d)
{
	int i;
	int j;

	i = -1;
	j = -1;
	if (two_d)
	{
		while (two_d[++i])
			free(two_d[i]);
		free(two_d);
	}
	if (three_d)
	{
		while (three_d[++j])
			free_char_arr(three_d[j], NULL);
		free(three_d);
	}
}

void free_tokens(t_token *token)
{
	t_token	*tmp_token;

	while (token)
	{
		tmp_token = token;
		token = token->next;
		free(tmp_token->value);
		free(tmp_token);
	}
}

void	free_lst(t_lst *lst)
{
	while (lst->fwd)
	{
		lst = lst->fwd;
		free(lst->bwd->var_n_val);
		free(lst->bwd->var);
		free(lst->bwd->val);
		free(lst->bwd);
	}
	free(lst->var_n_val);
	free(lst->var);
	free(lst->val);
	free(lst);
}

void	free_tree(t_tree_node *n)
{
	t_args	*tmp_args_ptr;
	// printf("%s\n", n->value);
	free(n->value);
	while (n->cmd_args)
	{
		tmp_args_ptr = n->cmd_args;
		free(tmp_args_ptr->arg);
		free(tmp_args_ptr);
		n->cmd_args = n->cmd_args->next;
	}
	free_char_arr(n->cmd_args_arr, NULL);
	free(n->exec_cmd_path);
	traverse_tree(&n);
}

void	free_all(t_tree_node *n)
{
	printf("hits\n");
	while (n->type != END)
	{
		// free(n->infile);
		// free(n->outfile);
		// free(n->delimiter);
		free_char_arr(n->cmd_args_arr, NULL);
		traverse_tree(&n);
	}
	// free(p->filepaths);
	// free(p->pid);
	// free_int_array(p->fd, p->pipe_ct);
	// free_char_arr(p->cmd_filepaths, p->cmd_args);
	// free_char_arr(p->exec_cmd_path, NULL);
}
