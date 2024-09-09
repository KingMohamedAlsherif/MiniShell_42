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

#include "../minishell.h"

void	free_lst_node(t_lst *node)
{
	free (node->var_n_val);
	free (node->var);
	free (node->val);
	free (node);
}

void	free_int_array(int **twoD, int pipe_ct)
{
	int i;

	i = -1;
	if (twoD)
	{
		while (++i < pipe_ct)
			free(twoD[i]);
		free(twoD);
	}
}

void	free_char_arr(char **twoD, char ***threeD)
{
	int i;
	int j;

	i = -1;
	j = -1;
	if (twoD)
	{
		while (twoD[++i])
			free(twoD[i]);
		free(twoD);
	}
	if (threeD)
	{
		while (threeD[++j])
			free_char_arr(threeD[j], NULL);
		free(threeD);
	}
}

void	free_all(t_tree_node *n)
{
	while (!n->is_last_node)
	{
		free(n->infile);
		free(n->outfile);
		free(n->delimiter);
		free_char_arr(n->token->cmd_args_arr, NULL);
		traverse_tree(&n);
	}
	// free(p->filepaths);
	// free(p->pid);
	// free_int_array(p->fd, p->pipe_ct);
	// free_char_arr(p->cmd_filepaths, p->cmd_args);
	// free_char_arr(p->exec_cmd_path, NULL);
}
