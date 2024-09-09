/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 12:53:22 by chon              #+#    #+#             */
/*   Updated: 2024/09/09 16:27:47 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	insert_node(t_tree_node *n, char *str)
{
	t_lst	*env_node;
	t_lst	*export_node;
	t_lst	*tmp_node;
	int		rank;

	env_node = n->ms_env;
	while (env_node->fwd)
		env_node = env_node->fwd;
	env_node->fwd = new_node(ft_strdup(str), 0);
	update_order(n->ms_env, env_node);
	rank = env_node->ascii_order;
	export_node = n->ms_export;
	while (export_node->ascii_order < rank - 1)
		export_node = export_node->fwd;
	tmp_node = export_node->fwd;
	export_node->fwd = new_node(export_str(str), rank);
	export_node->fwd->fwd = tmp_node;
	export_node = tmp_node;
	while (export_node)
	{
		export_node->ascii_order++;
		export_node = export_node->fwd;
	}
	free(str);
}

void	export_update(t_tree_node *n, char *str)
{
	char	*new_str;
	t_lst	*env_node;
	t_lst	*export_node;
	char	**s_new_str;

	new_str = remove_quotes(str);
	s_new_str = ft_split(new_str, '=');
	env_node = n->ms_env;
	while (env_node)
	{
		if (!ft_strncmp(env_node->var, s_new_str[0], ft_strlen(s_new_str[0])))
		{
			free(env_node->var_n_val);
			free(env_node->val);
			env_node->var_n_val = ft_strdup(new_str);
			env_node->val = ft_strdup(s_new_str[1]);
			export_node = n->ms_export;
			while (export_node->ascii_order != env_node->ascii_order)
				export_node = export_node->fwd;
			free(export_node->var_n_val);
			// free(export_node->val);
			export_node->var_n_val = export_str(new_str);
			// export_node->val = export_str(new_str);
			return ;
		}
		env_node = env_node->fwd;
	}
	insert_node(n, new_str);
}

void	export(t_tree_node *n)
{
	t_lst	*export_node;
	int		i;

	if (!n->token->cmd_args_arr)
	{
		export_node = n->ms_export;
		while (export_node)
		{
			printf("%s", export_node->var_n_val);
			export_node = export_node->fwd;
		}
	}
	else
	{
		i = -1;
		while (n->token->cmd_args_arr[++i])
		{
			if (!has_valid_chars(n->token->cmd_args_arr[i]))
				printf("bash: export: `%s': not a valid identifier\n"
					, n->token->cmd_args_arr[i]);
			else
				export_update(n, n->token->cmd_args_arr[i]);
		}
	}
}
