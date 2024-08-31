#include "../minishell.h"

void	cd(t_tree_node *n)
{
	chdir(n->token->cmd_args[0]);
}