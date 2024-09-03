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

void	env(t_tree_node *n)
{
	if (!n->token->cmd_args)
	{
		while (n->ms_env)
		{
			if (ft_strchr(n->ms_env->str, '='))
				printf("%s", n->ms_env->str);
			n->ms_env = n->ms_env->next;
		}
	}
}

void	export(t_tree_node *n)
{
	int	i;

	if (!n->token->cmd_args)
	{
		while (n->ms_export)
		{
			printf("%s", n->ms_export->str);
			n->ms_export = n->ms_export->next;
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
			{
				if (ft_strchr(n->token->cmd_args[i], '='))
					
			}

		}
	}
}
