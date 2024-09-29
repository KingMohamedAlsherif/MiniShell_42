/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingmohamedalsherif <kingmohamedalsherif@s +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:32:47 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 17:07:48 by kingmohamedalshe ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_pwd(t_lst *pwd_ptr, t_lst *old_pwd_ptr, bool env_flag)
{
	char	cwd[PATH_MAX];

	while (ft_strncmp(pwd_ptr->var, "PWD", 4))
		pwd_ptr = pwd_ptr->fwd;
	while (ft_strncmp(old_pwd_ptr->var, "OLDPWD", 7))
		old_pwd_ptr = old_pwd_ptr->fwd;
	free(old_pwd_ptr->var_n_val);
	old_pwd_ptr->var_n_val = ft_strjoin("OLDPWD=", pwd_ptr->val, 0, 0);
	old_pwd_ptr->var_n_val = ft_strjoin(old_pwd_ptr->var_n_val, "\n", 1, 0);
	free(pwd_ptr->var_n_val);
	if (env_flag)
	{
		pwd_ptr->var_n_val = ft_strjoin("PWD=", getcwd(cwd, sizeof(cwd)), 0, 0);
		pwd_ptr->var_n_val = ft_strjoin(pwd_ptr->var_n_val, "\n", 1, 0);
	}
	else
	{
		pwd_ptr->var_n_val = ft_strjoin("PWD=\"", (getcwd(cwd, sizeof(cwd))), 0,
				0);
		pwd_ptr->var_n_val = ft_strjoin(pwd_ptr->var_n_val, "\"\n", 1, 0);
	}
	free(pwd_ptr->val);
	pwd_ptr->val = ft_strdup(getcwd(cwd, sizeof(cwd)));
}

void	cd(t_tree_node *n)
{
	if (chdir(n->cmd_args_arr[1]) < 0)
		ft_error(errno, ft_strjoin("cd: ", n->cmd_args_arr[0], 0, 0), n, 0);
	update_pwd(n->ms->env, n->ms->env, 1);
	update_pwd(n->ms->exp, n->ms->exp, 0);
}

void	pwd(t_tree_node *n)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		ft_error(errno, ft_strdup("cwd"), n, 1);
}

