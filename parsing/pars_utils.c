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
void add_arg(t_token **args, t_token *new_arg)
{
    if (*args == NULL)
    {
        *args = new_arg;
    }
    else
    {
        t_token *current = *args;
        while (current->next)
            current = current->next;
        current->next = new_arg;
    }
}