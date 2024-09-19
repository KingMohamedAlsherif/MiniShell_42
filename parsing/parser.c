#include "../minishell.h"

void	parse_redir(t_token **tokens, t_tree_node **ast)
{
	t_redir *new_redir;
	t_redir *current;

	new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
        exit (1);
    new_redir->filename = NULL;
    new_redir->is_heredoc = 0;
    new_redir->heredoc_delim = NULL;
    new_redir->is_append = 0;
    new_redir->regular_infile = 0;
    new_redir->regular_outfile = 0;
    new_redir->next = NULL;
    if (!(*ast)->redir)
        (*ast)->redir = new_redir;
    else
    {
        current = (*ast)->redir;
        while (current->next)
            current = current->next;
        current->next = new_redir;
    }
	update_node(new_redir, (*tokens)->next->value, (*tokens)->type);
	*tokens = (*tokens)->next;
    // printf("Current token value: %s\n", (*token)->value);
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
    t_tree_node	*new_t_node;

	if (!*ast || (*ast)->token->type == PIPE || token->type == END)
	{
		new_t_node = init_tree_node(token);
		add_cmd_arg(&new_t_node->token->cmd_args, token->value);
		if (*ast)
		{
			if ((*ast)->token->type == PIPE && token->type == END)
				(*ast) = (*ast)->right;
			(*ast)->right = new_t_node;
			new_t_node->parent = *ast;
		}
		*ast = new_t_node;
	}
	else
		add_cmd_arg(&((*ast)->token->cmd_args), token->value);
}

void	parse(t_token **tokens, t_tree_node **ast)
{
    if ((*tokens)->type == PIPE)
		parse_pipe(*tokens, ast);
    else if ((*tokens)->type >= REDIRECT_IN && (*tokens)->type <= APPEND)
        parse_redir((tokens), ast);
    else if (*tokens)
		parse_word(*tokens, ast);
	if ((*tokens)->next)
		parse(&(*tokens)->next, ast);
}
