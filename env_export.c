/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:24:09 by chon              #+#    #+#             */
/*   Updated: 2024/09/28 22:56:24 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*export_str(char *str)
{
	char	*str_0;
	char	*str_2;
	char	*str_3;
	int		substr_len;

	str_0 = ft_strtrim(str, "\n");
	if (ft_strchr(str, '='))
	{
		substr_len = ft_strchr(str_0, '=') - str_0 + 1;
		str_2 = ft_substr(str_0, 0, substr_len);
		str_2 = ft_strjoin(str_2, "\"", 1, 0);
		str_3 = ft_strjoin(ft_substr(str_0, substr_len, ft_strlen(str_0)), "\"", 1, 0);
		free(str_0);
		str_0 = ft_strjoin(str_2, str_3, 1, 1);
	}
	return (str_0);
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
		// printf("val in exp: %s\n", exp_head_ptr->fwd->val);
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
			return ;
	}
	(*ms_env)[str_ct] = NULL;
}

void	update_order(t_lst *head, t_lst *node)
{
	int	rank;
	int	node_str_len;
	int i;

	i = -1;
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
		return ;
	(*ms)->env = create_new_node(ft_strdup(env[0]), 0);
	head_ptr = (*ms)->env;
	i = 0;
	while (env[++i])
	{
		if (strncmp(env[i], "LD_", 3) && strncmp(env[i], "GLIBC", 5))
		{
			new_node = create_new_node(ft_strdup(env[i]), 0);
			// printf("value: %s\n", new_node->val);
			if (!ft_strncmp(env[i], "OLDPWD=", 7))
			{
				free(new_node->var_n_val);
				new_node->var_n_val = ft_strdup("OLDPWD");
			}
			new_node->bwd = (*ms)->env;
			(*ms)->env->fwd = new_node;
			(*ms)->env = (*ms)->env->fwd;
		}
	}
	new_node = create_new_node(ft_strdup("?=0"), 0);
	new_node->bwd = (*ms)->env;
	(*ms)->env->fwd = new_node;
	(*ms)->env = head_ptr;
	while (head_ptr)
	{
		update_order((*ms)->env, head_ptr);
		head_ptr = head_ptr->fwd;
	}
	create_ms_env_arr(&(*ms)->env_arr, env);
	create_ms_export(&(*ms)->exp, (*ms)->env);
}
