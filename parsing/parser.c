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

void	parse_redir(t_token **tokens, t_redir *redir)
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
    if (!redir)
        redir = new_redir;
    else
    {
        while (redir->fwd)
            redir = redir->fwd;
        redir->fwd = new_redir;
		new_redir->bwd = redir;
    }
	update_node(new_redir, (*tokens)->next->value, (*tokens)->type);
	*tokens = (*tokens)->next;
}

void	parse_pipe(t_token  *token, t_tree_node     **ast)
{
    t_tree_node *new_t_node;

	while ((*ast)->parent)
		*ast = (*ast)->parent;
	new_t_node = init_tree_node(token);
    new_t_node->left = *ast;
	(*ast)->parent = new_t_node;
    *ast = new_t_node;
}

void	parse_word(t_token *token, t_tree_node **ast)
{
	if ((*ast)->token->type == PIPE || token->type == END)
	{
		if ((*ast)->token->type == PIPE && token->type == END)
			(*ast) = (*ast)->right;
		(*ast)->right = init_tree_node(token);
		(*ast)->right->parent = *ast;
		*ast = new_t_node;
	}
	else
	{
		if ((*ast)->token->type != WORD)
		{
			(*ast)->token = token;
			add_cmd_arg(&new_t_node->token->cmd_args, token->value);
		}
		else
			add_cmd_arg(&((*ast)->token->cmd_args), token->value);
	}
}

void	parse(t_token **tokens, t_tree_node **ast, t_ms_var *ms)
{
	if ((*tokens)->type == PIPE)
		parse_pipe(*tokens, ast);
    else if ((*tokens)->type >= REDIRECT_IN && (*tokens)->type <= APPEND)
        parse_redir(tokens, (*ast)->redir);
    else if (*tokens)
		parse_word(*tokens, ast);
	if ((*tokens)->next)
		parse(&(*tokens)->next, ast);
}
