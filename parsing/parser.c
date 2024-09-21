#include "../minishell.h"

void	update_node(t_redir *new_redir, char *value, token_type type)
{
    new_redir->filename = ft_strdup(value);
    if (type == HEREDOC)
        new_redir->heredoc_delim = ft_strdup(value);
    else if (type == APPEND)
        new_redir->is_append = 1;
    else if (type == REDIRECT_IN)
        new_redir->in_fd = 1;
    else if (type == REDIRECT_OUT)
        new_redir->out_fd = 1;
}

void	parse_redir(t_token **tokens, t_redir **redir)
{
	t_redir *new_redir;

	new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        exit (1);
    new_redir->filename = NULL;
    new_redir->heredoc_delim = NULL;
    new_redir->in_fd = 0;
    new_redir->is_append = 0;
    new_redir->out_fd = 0;
    new_redir->fwd = NULL;
    new_redir->bwd = NULL;
    if (!*redir)
        *redir = new_redir;
    else
    {
        while ((*redir)->fwd)
            *redir = (*redir)->fwd;
        (*redir)->fwd = new_redir;
		new_redir->bwd = *redir;
    }
	update_node(new_redir, (*tokens)->next->value, (*tokens)->type);
	*tokens = (*tokens)->next;
}

void	parse_pipe(t_token **tokens, t_tree_node **ast, t_ms_var *ms)
{
	while ((*ast)->parent)
		*ast = (*ast)->parent;
	(*ast)->parent = init_tree_node(*tokens, NULL);
    (*ast)->parent->left = *ast;
    *ast = (*ast)->parent;
	*tokens = (*tokens)->next;
	(*ast)->right = init_tree_node(*tokens, ms);
	(*ast)->right->parent = *ast;
	*ast = (*ast)->right;
	if ((*tokens)->type == WORD)
		add_cmd_arg(&((*ast)->cmd_args), (*tokens)->value);
}

void	parse_word(t_token *token, t_tree_node **ast, t_ms_var *ms)
{
	if (token->type == END)
	{
		(*ast)->right = init_tree_node(token, ms);
		(*ast)->right->parent = *ast;
		*ast = (*ast)->right;
	}
	if (token->type != END)
		add_cmd_arg(&((*ast)->cmd_args), token->value);
}

void	parse(t_token **tokens, t_tree_node **ast, t_ms_var *ms)
{
	if ((*tokens)->type == PIPE)
		parse_pipe(tokens, ast, ms);
    else if ((*tokens)->type >= REDIRECT_IN && (*tokens)->type <= APPEND)
        parse_redir(tokens, &(*ast)->redir);
    else if (*tokens)
		parse_word(*tokens, ast, ms);
	if ((*tokens)->next)
		parse(&(*tokens)->next, ast, ms);
}
