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
	if (chdir(n->cmd_args_arr[0]) < 0)
		ft_error(errno, ft_strjoin("cd: ", n->cmd_args_arr[0], 0, 0), n, 1);
	// update pwd and oldpwd
}

void	get_cwd(t_tree_node *n)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("Current working directory: %s\n", cwd);
	else
		ft_error(errno, ft_strdup("cwd"), n, 1);
}

void	unset(t_tree_node *n)
{
	char	*new_str;
	int		i;
	t_lst	*env_node;

	i = -1;
	while (n->cmd_args_arr[++i])
	{
		env_node = n->ms->env;
		new_str = remove_quotes(n->cmd_args_arr[i]);
		while (env_node)
		{
			if (!ft_strncmp(env_node->var, new_str, ft_strlen(new_str) + 1))
			{
				del_node(n->ms->env, env_node->ascii_order);
				del_node(n->ms->exp, env_node->ascii_order);
				break ;
			}
			env_node = env_node->fwd;
		}
		free(new_str);
	}
}

void	env(char *arg, t_lst *env)
{
	if (!arg)
	{
		while (env)
		{
			if (ft_strchr(env->var_n_val, '='))
				printf("%s", env->var_n_val);
			env = env->fwd;
		}
	}
	else
		printf("env: '%s': No such file or directory\n", arg);
}

bool	is_builtin(t_tree_node *n, char *cmd)
{
	if (ft_strlen(cmd) == 6 && !ft_strncmp(cmd, "export", 7))
		return (export(n), 1);
	else if (ft_strlen(cmd) == 2 && !ft_strncmp(cmd, "cd", 3))
		return (cd(n), 1);
	else if (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "env", 4))
		return (env(n->cmd_args_arr[1], n->ms->env), 1);
	else if (ft_strlen(cmd) == 5 && !ft_strncmp(cmd, "unset", 6))
		return (unset(n), 1);
	return (0);
}
