/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:32:47 by chon              #+#    #+#             */
/*   Updated: 2024/09/03 15:32:47 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd(t_tree_node *n)
{
	if (chdir(n->token->cmd_args_arr[0]) < 0)
		ft_error(errno, ft_strjoin("cd: ", n->token->cmd_args_arr[0]), n, 1);
}

void	get_cwd(t_tree_node *n)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("Current working directory: %s\n", cwd);
	else
		ft_error(errno, "cwd", n, 1);
}

void	unset(t_tree_node *n)
{
	char	*new_str;
	int		i;
	t_lst	*env_node;

	i = -1;
	while (n->token->cmd_args_arr[++i])
	{
		env_node = n->ms_env;
		new_str = remove_quotes(n->token->cmd_args_arr[i]);
		while (env_node)
		{
			if (!ft_strncmp(env_node->var, new_str, ft_strlen(new_str) + 1))
			{
				del_node(n->ms_env, env_node->ascii_order);
				del_node(n->ms_export, env_node->ascii_order);
				break ;
			}
			env_node = env_node->fwd;
		}
	}
}

void	env(t_tree_node *n)
{
	if (!n->token->cmd_args_arr)
	{
		while (n->ms_env)
		{
			if (ft_strchr(n->ms_env->var, '='))
				printf("%s", n->ms_env->var_n_val);
			n->ms_env = n->ms_env->fwd;
		}
	}
}
