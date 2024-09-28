/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:53:22 by chon              #+#    #+#             */
/*   Updated: 2024/09/28 22:45:07 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	insert_node(t_ms_var *ms, char *str)
{
	t_lst	*env_node;
	t_lst	*exp_node;
	t_lst	*tmp_node;
	int		rank;

	env_node = ms->env;
	while (env_node->fwd)
		env_node = env_node->fwd;
	env_node->fwd = create_new_node(ft_strdup(str), 0);
	env_node->fwd->bwd = env_node;
	update_order(ms->env, env_node->fwd);
	rank = env_node->fwd->ascii_order;
	exp_node = ms->exp;
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

void	update_tlst(t_lst *env_node, t_lst *exp_node, char *str, char **s_str)
{
	while (exp_node->ascii_order != env_node->ascii_order)
		exp_node = exp_node->fwd;
	free(env_node->var_n_val);
	if (env_node->val)
		free(env_node->val);
	free(exp_node->var_n_val);
	if (s_str[1])
	{
		env_node->var_n_val = ft_strjoin(ft_strdup(str), "\n", 1, 0);
		env_node->val = ft_strdup(s_str[1]);
		exp_node->var_n_val = ft_strjoin(export_str(str), "\n", 1, 0);
	}
	else
	{
		env_node->var_n_val = ft_strjoin(ft_strdup(str), "\"\"\n", 1, 0);
		env_node->val = NULL;
		exp_node->var_n_val = ft_strjoin(ft_strdup(str), "\"\"\n", 1, 0);
		exp_node->var_n_val = ft_strjoin("declare -x ", exp_node->var_n_val, 0, 1);
	}
	free_char_arr(s_str, NULL);
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
	insert_node(n->ms, new_str);
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
				printf("-bash: export: `%s': not a valid identifier\n"
					, n->cmd_args_arr[i]);
			else
				env_export_update(n, n->cmd_args_arr[i]);
		}
	}
}
