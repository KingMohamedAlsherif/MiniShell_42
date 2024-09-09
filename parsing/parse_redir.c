#include "../minishell.h"

void redir_in(t_redir **redir, char *filename)
{
    t_redir *tmp;
    t_redir *new_redir;

    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        return;

    new_redir->filename = filename;
    new_redir->is_heredoc = 0;
    new_redir->heredoc_delim = NULL;
    new_redir->is_append = 0;
    new_redir->regular_infile = 1;
    new_redir->regular_outfile = 0;
    new_redir->next = NULL;
    if (*redir == NULL)
        *redir = new_redir;
    else
    {
        tmp = *redir;
        while (tmp)
            tmp = tmp->next;
        tmp->next = new_redir;
    }
}

void redir_out(t_redir **redir, char *filename)
{
    t_redir *tmp;
    t_redir *new_redir;

    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        return;

    new_redir->filename = filename;
    new_redir->is_heredoc = 0;
    new_redir->heredoc_delim = NULL;
    new_redir->is_append = 0;
    new_redir->regular_infile = 0;
    new_redir->regular_outfile = 1;
    new_redir->next = NULL;
    if (*redir == NULL)
        *redir = new_redir;
    else
    {
        tmp = *redir;
        while (tmp)
            tmp = tmp->next;
        tmp->next = new_redir;
    }
}

void redir_heredoc(t_redir **redir, char *heredoc_d)
{
    t_redir *tmp;
    t_redir *new_redir;

    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        return;

    new_redir->filename = NULL;
    new_redir->is_heredoc = 1;
    new_redir->heredoc_delim = heredoc_d;
    new_redir->is_append = 0;
    new_redir->regular_infile = 1;
    new_redir->regular_outfile = 0;
    new_redir->next = NULL;
    if (*redir == NULL)
        *redir = new_redir;
    else
    {
        tmp = *redir;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_redir;
    }
}

void redir_append(t_redir **redir, char *filename)
{
    t_redir *tmp;
    t_redir *new_redir;

    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        return;

    new_redir->filename = filename;
    new_redir->is_heredoc = 0;
    new_redir->heredoc_delim = NULL;
    new_redir->is_append = 1;
    new_redir->regular_infile = 0;
    new_redir->regular_outfile = 1;
    new_redir->next = NULL;
    if (*redir == NULL)
        *redir = new_redir;
    else
    {
        tmp = *redir;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_redir;
    }
}