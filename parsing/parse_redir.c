#include "../minishell.h"

static void add_redir(t_redir **redir, t_redir *new_redir)
{
    t_redir *current;

    if (*redir == NULL)
    {
        *redir = new_redir;
    }
    else
    {
        current = *redir;
        while (current && current->next)
            current = current->next;
        if (current)
            current->next = new_redir;
    }
}

t_redir         *init_redir(t_redir  **redir)
{
    (void)redir;
    t_redir *new_redir;

    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        return (NULL);
    new_redir->filename = NULL;
    new_redir->is_heredoc = 0;
    new_redir->heredoc_delim = NULL;
    new_redir->is_append = 0;
    new_redir->regular_infile = 0;
    new_redir->regular_outfile = 0;
    new_redir->next = NULL;
    return (new_redir);
}
void        handle_redir(t_redir    **redir, char   *value, token_type   type)
{
    t_redir *new_redir;

    new_redir = init_redir(redir);
    if (type == HEREDOC)
    {
        new_redir->heredoc_delim = value;
        new_redir->is_heredoc = 1;
    }
    else if (type == APPEND)
    {
        new_redir->filename = value;
        new_redir->is_append = 1;
    }
    else if (type == REDIRECT_IN)
    {
        new_redir->filename = value;
        new_redir->regular_infile = 1;
    }
    else if (type == REDIRECT_OUT)
    {
        new_redir->filename = value;
        new_redir->regular_outfile = 1;
    }
    add_redir(redir, new_redir);
}
