/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:24:09 by chon              #+#    #+#             */
/*   Updated: 2024/09/03 14:29:05 by chon             ###   ########.fr       */
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

void	create_ms_export(t_export **head, t_env *ms_env)
{
	t_export	*head_ptr;
	int			i;
	t_env		*env_head;
	int			node_ct;

	head_ptr = *head;
	env_head = ms_env;
	node_ct = count_lst_nodes(ms_env);
	i = -1;
	while (++i < node_ct)
	{
		while (ms_env->ascii_order != i)
			ms_env = ms_env->next;
		head_ptr->str = ft_strjoin("declare -x ", ms_env->str);
		ms_env = env_head;
		head_ptr->next = malloc(sizeof(t_export));
		if (!head_ptr->next)
			return ;
		head_ptr = head_ptr->next;
	}
}

void	update_order(t_env *head, t_env *node)
{
	int	rank;
	int	node_str_len;
	int i = -1;

	rank = 0;
	node_str_len = ft_strlen(node->str) + 1;
	while (head)
	{
		if (ft_strncmp(node->str, head->str, node_str_len) > 0)
			rank++;
		else if (ft_strncmp(node->str, head->str, node_str_len) < 0)
			node->ascii_order++;
		head = head->next;
		i++;
	}
	node->ascii_order = rank;
}

void	create_ms_env(t_env **head, char **env)
{
	t_env	*head_ptr;
	int		i;

	head_ptr = *head;
	i = -1;
	while (env[++i])
	{
		head_ptr->str = ft_strdup(env[i]);
		head_ptr->ascii_order = 0;
		if (env[i + 1])
		{
			head_ptr->next = malloc(sizeof(t_env));
			if (!head_ptr->next)
				return ;
		}
		else	
			head_ptr->next = NULL;
		head_ptr = head_ptr->next;
	}
	head_ptr = *head;
	while (head_ptr)
	{
		update_order(*head, head_ptr);
		head_ptr = head_ptr->next;
	}
}

void	create_env_export(t_env **env_head, t_export **export_head, char **env)
{
	*env_head = malloc(sizeof(t_env));
	if (!env_head)
		return ;
	*export_head = malloc(sizeof(t_export));
	if (!export_head)
		return ;
	create_ms_env(env_head, env);
	create_ms_export(export_head, *env_head);
}
