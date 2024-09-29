/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:53:22 by chon              #+#    #+#             */
/*   Updated: 2024/09/30 00:48:15 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	insert_node(t_ms_var *ms, char *str, t_lst *env_node, t_lst *exp_node)
{
	t_lst	*tmp_node;
	int		rank;

	while (env_node->fwd)
		env_node = env_node->fwd;
	env_node->fwd = create_new_node(ft_strdup(str), 0);
	env_node->fwd->bwd = env_node;
	update_order(ms->env, env_node->fwd);
	rank = env_node->fwd->ascii_order;
	while (exp_node->ascii_order < rank - 1)
		exp_node = exp_node->fwd;
	tmp_node = exp_node->fwd;
	exp_node->fwd = create_new_node(export_str(str), rank);
	exp_node->fwd->bwd = exp_node;
	exp_node->fwd->fwd = tmp_node;
	if (tmp_node)
		tmp_node->bwd = exp_node->fwd;
	exp_node = tmp_node;
	while (exp_node)
	{
		exp_node->ascii_order++;
		exp_node = exp_node->fwd;
	}
	free(str);
}

void	env_export_update(t_tree_node *n, char *str)
{
	char	*new_str;
	t_lst	*env_node;
	char	**s_new_str;

	new_str = remove_quotes(str);
	s_new_str = ft_split(new_str, '=');
	env_node = n->ms->env;
	while (env_node)
	{
		if (!ft_strncmp(env_node->var, s_new_str[0], ft_strlen(s_new_str[0])))
		{
			update_tlst(env_node, n->ms->exp, new_str, s_new_str);
			return ;
		}
		env_node = env_node->fwd;
	}
	free_char_arr(s_new_str, NULL);
	insert_node(n->ms, new_str, n->ms->env, n->ms->exp);
}

void	export(t_tree_node *n)
{
	t_lst	*exp_node;
	int		i;

	if (!n->cmd_args_arr[1])
	{
		exp_node = n->ms->exp;
		while (exp_node)
		{
			if (ft_strncmp(exp_node->var, "declare -x ?", 13))
				printf("declare -x %s", exp_node->var_n_val);
			exp_node = exp_node->fwd;
		}
	}
	else
	{
		i = 0;
		while (n->cmd_args_arr[++i])
		{
			if (!has_valid_chars(n->cmd_args_arr[i]))
				printf("-bash: export: `%s': not a valid identifier\n",
					n->cmd_args_arr[i]);
			else
				env_export_update(n, n->cmd_args_arr[i]);
		}
	}
}
