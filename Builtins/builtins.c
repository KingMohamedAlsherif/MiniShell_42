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

void cd(t_tree_node *n)
{
	if (!n->cmd_args_arr[1])
	{
		if (chdir("/") < 0)
			ft_error(errno, ft_strjoin("cd: ", n->cmd_args_arr[0], 0, 0), n, 0);
	}
	else if (n->cmd_args_arr[2])
		printf("Minishell: cd: too many arguments\n");
	else if (chdir(n->cmd_args_arr[1]) < 0)
		ft_error(errno, ft_strjoin("cd: ", n->cmd_args_arr[0], 0, 0), n, 0);
	// update pwd and oldpwd
}

void pwd(t_tree_node *n)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("Current working directory: %s\n", cwd);
	else
		ft_error(errno, ft_strdup("cwd"), n, 1);
}

void unset(t_tree_node *n)
{
	char *new_str;
	int i;
	t_lst *env_node;
	int ascii_order;

	i = -1;
	while (n->cmd_args_arr[++i])
	{
		env_node = n->ms->env;
		new_str = remove_quotes(n->cmd_args_arr[i]);
		while (env_node)
		{
			ascii_order = env_node->ascii_order;
			// printf("rank: %d\n", env_node->ascii_order);
			if (!ft_strncmp(env_node->var, new_str, ft_strlen(new_str) + 1))
			{
				del_node(n->ms->env, ascii_order);
				del_node(n->ms->exp, ascii_order);
				break;
			}
			env_node = env_node->fwd;
		}
		free(new_str);
	}
}

void env(char *exec_cmd_path, char *arg, t_lst *env)
{
	if (ft_strncmp(exec_cmd_path, "PATH", 5) && !arg)
	{
		printf("env cmd path: %s\n", exec_cmd_path);
		while (env)
		{
			if (ft_strchr(env->var_n_val, '=') && !ft_strchr(env->var, '?'))
				printf("%s", env->var_n_val);
			env = env->fwd;
		}
	}
	else if (ft_strncmp(exec_cmd_path, "PATH", 5))
		printf("env: '%s': No such file or directory\n", arg);
	else
		printf("Minishell: env: No such file or directory\n");
}

void execute_builtin(t_tree_node *n, char *cmd, bool exit_flag)
{
	if (ft_strlen(cmd) == 2 && !ft_strncmp(cmd, "cd", 3))
		cd(n);
	else if (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "pwd", 4))
		pwd(n);
	else if (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "env", 4))
		env(n->exec_cmd_path, n->cmd_args_arr[1], n->ms->env);
	else if (ft_strlen(cmd) == 5 && !ft_strncmp(cmd, "unset", 6))
		unset(n);
	if (ft_strlen(cmd) == 6 && !ft_strncmp(cmd, "export", 7))
		export(n);
	ft_error(0, ft_strdup(n->cmd_args_arr[0]), n, exit_flag);
}

bool is_builtin(char *cmd)
{
	if ((ft_strlen(cmd) == 2 && (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd, "$?", 3))) || (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "pwd", 4)) || (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "env", 4)) || (ft_strlen(cmd) == 5 && !ft_strncmp(cmd, "unset", 6)) || (ft_strlen(cmd) == 6 && !ft_strncmp(cmd, "export", 7)))
		return (1);
	return (0);
}
