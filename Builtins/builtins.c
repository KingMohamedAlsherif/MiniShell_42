#include "../minishell.h"

void	cd(t_tree_node *n)
{
	if (chdir(n->token->cmd_args[0]) < 0)
		ft_error(errno, ft_strjoin("cd: ",n->token->cmd_args[0]), n, 1);
}

void	get_cwd(t_tree_node *n)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("Current working directory: %s\n", cwd);
	else
		ft_error(errno, "cwd", n, 1);
}