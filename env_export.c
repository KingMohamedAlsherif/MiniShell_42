/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:24:09 by chon              #+#    #+#             */
/*   Updated: 2024/09/09 16:49:39 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	create_ms_env(char ***ms_env, char **env)
// {
// 	char	**ms_env_ptr;
// 	int		str_ct;

// 	str_ct = 0;
// 	while (env[str_ct])
// 		str_ct++;
// 	ms_env_ptr = malloc(sizeof(char *) * (str_ct + 1));
// 	if (!ms_env_ptr)
// 		return ;
// 	str_ct = -1;
// 	while (env[++str_ct])
// 		ms_env_ptr[str_ct] = ft_strdup(env[str_ct]);
// 	ms_env_ptr[str_ct] = NULL;
// 	*ms_env = ms_env_ptr;
// }

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
		// free_char_arr(split_str, NULL);
		// free(tmp_str_2);
		free(export_str);
		export_str = ft_strjoin(tmp_str, tmp_str_3);
		// free(tmp_str);
		// free(tmp_str_3);
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

void	create_env_export(t_lst **env_head, t_lst **export_head, char **env)
{
	t_lst	*head_ptr;
	int		i;

	*env_head = new_node(ft_strdup(env[0]), 0);
	head_ptr = *env_head;
	i = 0;
	while (env[++i])
	{
		(*env_head)->fwd = new_node(ft_strdup(env[i]), 0);
		*env_head = (*env_head)->fwd;
		(*env_head)->bwd = *env_head;
	}
	*env_head = head_ptr;
	while (head_ptr)
	{
		update_order(*env_head, head_ptr);
		head_ptr = head_ptr->fwd;
	}
	// printf("%s=%s\n", (*env_head)->fwd->var, (*env_head)->fwd->val);
	create_ms_export(export_head, *env_head);
	// printf("%s=%s\n", (*env_head)->var, (*env_head)->val);
}
