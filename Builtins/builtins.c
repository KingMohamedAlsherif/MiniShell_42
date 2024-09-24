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
		ft_exit(errno, ft_strjoin("cd: ", n->cmd_args_arr[0], 0, 0), n, 1);
	// update pwd and oldpwd
}

void	get_cwd(t_tree_node *n)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("Current working directory: %s\n", cwd);
	else
		ft_exit(errno, ft_strdup("cwd"), n, 1);
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

void	env(t_lst *env, char *arg)
{
	if (!arg)
	{
		while (env)
		{
			if (ft_strchr(env->var_n_val, '=') && !ft_strchr(env->var, '?'))
				printf("%s", env->var_n_val);
			env = env->fwd;
		}
	}
	else
		printf("env: '%s': No such file or directory\n", arg);
}

void	execute_builtin(t_tree_node *n, char *cmd)
{
	t_lst	*env_ptr;

	if (ft_strlen(cmd) == 2 && !ft_strncmp(cmd, "cd", 3))
		cd(n);
	else if (ft_strlen(cmd) == 2 && !ft_strncmp(cmd, "$?", 3))
	{
		env_ptr = n->ms->env;
		while (env_ptr)
		{
			if (ft_strncmp(env_ptr->var, "?", 2))
				break ;
			env_ptr = env_ptr->fwd;
		}
		printf("%s\n", env_ptr->val);
	}
	else if (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "env", 4))
		env(n->ms->env, n->cmd_args_arr[1]);
	else if (ft_strlen(cmd) == 5 && !ft_strncmp(cmd, "unset", 6))
		unset(n);
	if (ft_strlen(cmd) == 6 && !ft_strncmp(cmd, "export", 7))
		export(n);
	ft_exit(0, ft_strdup(n->cmd_args_arr[0]), n, 1);
}

bool	is_builtin(char *cmd)
{
	if ((ft_strlen(cmd) == 2 && (!ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "$?", 3)))
		|| (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "env", 4))
		|| (ft_strlen(cmd) == 5 && !ft_strncmp(cmd, "unset", 6))
		|| (ft_strlen(cmd) == 6 && !ft_strncmp(cmd, "export", 7)))
		return (1);
	return (0);
}
