#include "../minishell.h"

void	free_tokens(t_token *token)
{
	t_token	*tmp_token;

	while (token)
	{
		tmp_token = token;
		token = token->next;
		free(tmp_token->value);
		free(tmp_token);
	}
}

void	add_cmd_arg(t_args **args_lst, char *value)
{
    t_args	*new_arg;
    t_args	*current;

	new_arg = malloc(sizeof(t_args));
	if (!new_arg)
		return ;
    new_arg->arg = ft_strdup(value);
    new_arg->next = NULL;
    if (!*args_lst)
        *args_lst = new_arg;
    else
    {
        current = *args_lst;
        while (current->next)
            current = current->next;
        current->next = new_arg;
    }
}

void    init_tree_paths(t_tree_node *new_node)
{
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
}

t_tree_node	*init_tree_node(t_token *token, t_ms_var *ms)
{
    t_tree_node	*new_node;

    new_node = malloc(sizeof(t_tree_node));
    if (!new_node)
        return (NULL);
    if (token->type == PIPE || token->type == END)
        new_node->type = token->type;
    else
        new_node->type = WORD;
    new_node->value = NULL;
    if (token->type == WORD)
	    new_node->value = ft_strdup(token->value);
	new_node->cmd_args = NULL;
	new_node->cmd_args_arr = NULL; 
	new_node->exec_cmd_path = NULL;
	new_node->p = NULL;
    new_node->redir = NULL;
    new_node->pipe_ct = 0;
    new_node->pipefd = NULL;
    init_tree_paths(new_node);
    new_node->is_read = 0;
	new_node->ms = ms;
    return (new_node);
}
