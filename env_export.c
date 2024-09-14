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

void	create_ms_env_arr(char ***ms_env, char **env)
{
	int		str_ct;

	str_ct = 0;
	while (env[str_ct])
		str_ct++;
	*ms_env = malloc(sizeof(char *) * (str_ct + 1));
	if (!*ms_env)
		return ;
	str_ct = -1;
	while (env[++str_ct])
		*ms_env[str_ct] = ft_strdup(env[str_ct]);
	*ms_env[str_ct] = NULL;
}

char	*export_str(char *str)
{
	char	**split_str;
	char	*tmp_str;
	char	*tmp_str_2;
	char	*tmp_str_3;
	char	*export_str;

	export_str = ft_strjoin(ft_strdup("declare -x "), str);
	if (ft_strchr(str, '='))
	{
		split_str = ft_split(str, '=');
		tmp_str = ft_strjoin(ft_strdup("declare -x "), split_str[0]);
		tmp_str_2 = ft_strjoin(ft_strdup("=\""), split_str[1]);
		tmp_str_3 = ft_strjoin(tmp_str_2, ft_strdup("\""));
		free(split_str);
		free(export_str);
		export_str = ft_strjoin(tmp_str, tmp_str_3);
	}
	if (!export_str)
		exit (1);
	return (export_str);
}

void	create_ms_export(t_lst **export_head, t_lst *ms_env)
{
	t_lst	*export_head_ptr;
	int		i;
	t_lst	*env_head;
	int		node_ct;

	env_head = ms_env;
	node_ct = count_lst_nodes(ms_env);
	while (ms_env->ascii_order)
		ms_env = ms_env->fwd;
	*export_head = new_node(export_str(ms_env->var_n_val), ms_env->ascii_order);
	export_head_ptr = *export_head;
	ms_env = env_head;
	i = 0;
	while (++i < node_ct)
	{
		while (ms_env->ascii_order != i)
			ms_env = ms_env->fwd;
		export_head_ptr->fwd = new_node(export_str(ms_env->var_n_val), ms_env->ascii_order);
		export_head_ptr = export_head_ptr->fwd;
		export_head_ptr->bwd = export_head_ptr;
		ms_env = env_head;
	}
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
	int		i;

	(*ms)->env = new_node(ft_strdup(env[0]), 0);
	head_ptr = (*ms)->env;
	i = 0;
	while (env[++i])
	{
		(*ms)->env->fwd = new_node(ft_strdup(env[i]), 0);
		(*ms)->env = (*ms)->env->fwd;
		(*ms)->env->bwd = (*ms)->env;
	}
	(*ms)->env->fwd = new_node(ft_strdup("$=4321"), 0);
	(*ms)->env = (*ms)->env->fwd;
	(*ms)->env->bwd = (*ms)->env;
	(*ms)->env = head_ptr;
	while (head_ptr)
	{
		update_order((*ms)->env, head_ptr);
		head_ptr = head_ptr->fwd;
	}
	// printf("%s=%s\n", (*ms)->env->fwd->var, (*ms)->env->fwd->val);
	create_ms_env_arr(&(*ms)->env_arr, env);
	create_ms_export(&(*ms)->exp, (*ms)->env);
	// printf("%s=%s\n", (*ms)->env->var, (*ms)->env->val);
}
