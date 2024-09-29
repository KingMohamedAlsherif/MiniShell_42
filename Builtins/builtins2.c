/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingmohamedalsherif <kingmohamedalsherif@s +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 09:17:25 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 17:12:12 by kingmohamedalshe ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env(char *exec_cmd_path, char *arg, t_lst *env)
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

void	execute_builtin(t_tree_node *n, char *cmd, bool exit_flag)
{
	if (ft_strlen(cmd) == 2 && !ft_strncmp(cmd, "cd", 3) && n->cmd_args_arr[1])
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

bool	is_builtin(char *cmd)
{
	if ((ft_strlen(cmd) == 2 && (!ft_strncmp(cmd, "cd", 3) || !ft_strncmp(cmd,
					"$?", 3))) || (ft_strlen(cmd) == 3 && !ft_strncmp(cmd,
				"pwd", 4)) || (ft_strlen(cmd) == 3 && !ft_strncmp(cmd, "env",
				4)) || (ft_strlen(cmd) == 5 && !ft_strncmp(cmd, "unset", 6))
		|| (ft_strlen(cmd) == 6 && !ft_strncmp(cmd, "export", 7)))
		return (1);
	return (0);
}

void	update_tlst(t_lst *env_node, t_lst *exp_node, char *str, char **s_str)
{
	while (exp_node->ascii_order != env_node->ascii_order)
		exp_node = exp_node->fwd;
	free(env_node->var_n_val);
	if (env_node->val)
		free(env_node->val);
	free(exp_node->var_n_val);
	if (s_str[1])
	{
		env_node->var_n_val = ft_strjoin(ft_strdup(str), "\n", 1, 0);
		env_node->val = ft_strdup(s_str[1]);
		exp_node->var_n_val = ft_strjoin(export_str(str), "\n", 1, 0);
	}
	else
	{
		env_node->var_n_val = ft_strjoin(ft_strdup(str), "\"\"\n", 1, 0);
		env_node->val = NULL;
		exp_node->var_n_val = ft_strjoin(ft_strdup(str), "\"\"\n", 1, 0);
	}
	free_char_arr(s_str, NULL);
	free(str);
}
