#include "../minishell.h"

t_args *create_arg_node(char *value)
{
    t_args *new_arg;

    new_arg = malloc(sizeof(t_args));
    if (!new_arg)
        return NULL;
    new_arg->arg = ft_strdup(value);
    new_arg->next = NULL;
    return new_arg;
}

void add_arg(t_args **args, t_args *new_arg)
{
    t_args *current;
   
    if (*args == NULL)
    {
        *args = new_arg;
    }
    else
    {
        current = *args;
        while (current->next)
            current = current->next;
        current->next = new_arg;
    }
}

int     init_cmd(t_token    *token, t_tree_node     **ast)
{
    t_tree_node *new_node;
    t_args      *new_arg;

    new_node = malloc(sizeof(t_tree_node));
    if (!new_node)
        return (MALLOC_ERROR);
    new_node->parent = NULL;
    new_node->type = CMD;
    new_node->token = token;
    new_node->left = NULL;
    new_node->right = NULL;
    new_arg = create_arg_node(new_node->token->value);
    *ast = new_node;
    if (!new_arg)
        return (MALLOC_ERROR);
    add_arg(&(new_node->token->cmd_args), new_arg);
    return (0);
}
// void append_end_node(t_tree_node *root)
// {
//     t_tree_node *current;

//     if (!root)
//         return;

//     current = root;

//     while (current->parent != NULL)
//         current = current->parent;

//     // Attach the END node as the right child of the rightmost node
//     current->right = create_end_node();
//     current->right = current; // Set parent to maintain tree structure
// }
