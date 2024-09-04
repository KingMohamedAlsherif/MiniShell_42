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
	if (chdir(n->token->cmd_args[0]) < 0)
		ft_error(errno, ft_strjoin("cd: ", n->token->cmd_args[0]), n, 1);
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
	while (n->token->cmd_args[++i])
	{
		env_node = n->ms_env;
		new_str = remove_quotes(n->token->cmd_args[i]);
		while (env_node)
		{
			if (!ft_strncmp(env_node->str, new_str, ft_strlen(new_str) + 1))
			{
				del_node(env_node);
				continue ;
			}
			env_node = env_node->fwd;
		}
	}
}

void	env(t_tree_node *n)
{
	if (!n->token->cmd_args)
	{
		while (n->ms_env)
		{
			if (ft_strchr(n->ms_env->str, '='))
				printf("%s", n->ms_env->str);
			n->ms_env = n->ms_env->fwd;
		}
	}
}

void	export_update(t_tree_node *n, char *str)
{
	char	*new_str;
	t_lst	*env_node;
	t_lst	*export_node;

	new_str = remove_quotes(str);
	env_node = n->ms_env;
	while (env_node)
	{
		if (!ft_strncmp(env_node->str, new_str, ft_strlen(new_str) + 1))
		{
			free(env_node->str);
			env_node->str = ft_strdup(new_str);
			export_node = n->ms_export;
			while (export_node->ascii_order != env_node->ascii_order)
				export_node = export_node->fwd;
			free(export_node);
			export_node->str = export_str(new_str);
			return ;
		}
		env_node = env_node->fwd;
	}
	insert_node(n, new_str);
}

void	export(t_tree_node *n)
{
	int		i;

	if (!n->token->cmd_args)
	{
		while (n->ms_export)
		{
			printf("%s", n->ms_export->str);
			n->ms_export = n->ms_export->fwd;
		}
	}
	else
	{
		i = -1;
		while (n->token->cmd_args[++i])
		{
			if (!has_valid_chars(n->token->cmd_args[i]))
				printf("bash: export: `%s': not a valid identifier\n"
					, n->token->cmd_args[i]);
			else
				export_update(n, n->token->cmd_args[i]);
		}
	}
}
