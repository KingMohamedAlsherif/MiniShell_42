/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:24:09 by chon              #+#    #+#             */
/*   Updated: 2024/09/13 16:55:08 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*export_str(char *str)
{
	char	**split_str;
	char	*tmp_str;
	char	*tmp_str_2;
	char	*tmp_str_3;
	char	*export_str;

	export_str = ft_strjoin("declare -x ", str);
	if (ft_strchr(str, '='))
	{
		split_str = ft_split(str, '=');
		tmp_str = ft_strjoin("declare -x ", split_str[0]);
		tmp_str_2 = ft_strjoin("=\"", split_str[1]);
		tmp_str_3 = ft_strjoin(tmp_str_2, "\"");
		free(tmp_str_2);
		free_char_arr(split_str, NULL);
		free(export_str);
		export_str = ft_strjoin(tmp_str, tmp_str_3);
		free(tmp_str);
		free(tmp_str_3);
	}
	if (!export_str)
		exit (1);
	return (export_str);
}

void	create_ms_export(t_lst **exp_head, t_lst *env)
{
	t_lst	*exp_head_ptr;
	int		i;
	t_lst	*env_head;
	int		node_ct;

	env_head = env;
	node_ct = count_lst_nodes(env);
	while (env->ascii_order)
		env = env->fwd;
	*exp_head = create_new_node(export_str(env->var_n_val), env->ascii_order);
	exp_head_ptr = *exp_head;
	env = env_head;
	i = 0;
	while (++i < node_ct)
	{
		while (env->ascii_order != i)
			env = env->fwd;
		exp_head_ptr->fwd = create_new_node(export_str(env->var_n_val), env->ascii_order);
		exp_head_ptr->fwd->bwd = exp_head_ptr;
		exp_head_ptr = exp_head_ptr->fwd;
		env = env_head;
	}
}

void	create_ms_env_arr(char ***ms_env, char **env)
{
	int		str_ct;

	str_ct = 0;
	while (env[str_ct])
		str_ct++;
	*ms_env = malloc((sizeof(char *) + 1) * str_ct);
	str_ct = -1;
	while (env[++str_ct])
	{
		(*ms_env)[str_ct] = ft_strdup(env[str_ct]);
		if (!(*ms_env)[str_ct])
			exit (1);
	}
	(*ms_env)[str_ct] = NULL;
}

void	update_order(t_lst *head, t_lst *node)
{
	int	rank;
	int	node_str_len;
	int i = -1;

	rank = 0;
	node_str_len = ft_strlen(node->var) + 1;
	while (head)
	{
		if (ft_strncmp(node->var, head->var, node_str_len) > 0)
			rank++;
		else if (ft_strncmp(node->var, head->var, node_str_len) < 0)
			node->ascii_order++;
		head = head->fwd;
		i++;
	}
	node->ascii_order = rank;
}

void	dup_env_exp(t_ms_var **ms, char **env)
{
	t_lst	*head_ptr;
	t_lst	*new_node;
	int		i;

	*ms = malloc(sizeof(t_ms_var));
	if (!*ms)
		exit (1);
	(*ms)->env = create_new_node(ft_strdup(env[0]), 0);
	head_ptr = (*ms)->env;
	i = 0;
	while (env[++i])
	{
		new_node = create_new_node(ft_strdup(env[i]), 0);
		new_node->bwd = (*ms)->env;
		(*ms)->env->fwd = new_node;
		(*ms)->env = (*ms)->env->fwd;
	}
	// new_node = create_new_node(ft_strdup("$=4321"), 0);
	// new_node->bwd = (*ms)->env;
	// (*ms)->env->fwd = new_node;
	(*ms)->env = head_ptr;
	while (head_ptr)
	{
		update_order((*ms)->env, head_ptr);
		head_ptr = head_ptr->fwd;
	}
	create_ms_env_arr(&(*ms)->env_arr, env);
	create_ms_export(&(*ms)->exp, (*ms)->env);
}
