#include "../minishell.h"

void	update_node(t_redir *new_redir, char *value, token_type type)
{
    if (type == HEREDOC)
    {
        new_redir->heredoc_delim = ft_strdup(value);
        new_redir->is_heredoc = 1;
    }
    else if (type == APPEND)
    {
        new_redir->filename = ft_strdup(value);
        new_redir->is_append = 1;
    }
    else if (type == REDIRECT_IN)
    {
        new_redir->filename = ft_strdup(value);
        new_redir->regular_infile = 1;
    }
    else if (type == REDIRECT_OUT)
    {
        new_redir->filename = ft_strdup(value);
        new_redir->regular_outfile = 1;
    }
}
