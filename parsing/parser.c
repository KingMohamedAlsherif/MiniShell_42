#include "../minishell.h"

void	add_end_node(t_tree_node **ast)
{
	t_token		*end_token;
    t_tree_node	*end_node;

    end_token = malloc(sizeof(t_token));
	if (!end_token)
		exit (1);
	end_token->type = END;
	end_node = NULL;
    // printf("%s\n", (*ast)->value);
    init_tree_node(&end_node, end_token);
	while ((*ast)->right)
        (*ast) = (*ast)->right;
    (*ast)->right = end_node;
}

int	parse_redir(t_token **token, t_tree_node **ast)
{
    if (!token || !*token || !ast || !*ast || !(*token)->next) // ensure that next token is word
        return (SYNTAX_ERROR);
    if ((*token)->type == REDIRECT_IN)
        handle_redir(&((*ast)->redir), (*token)->next->value, (*token)->type);
    else if ((*token)->type == REDIRECT_OUT)
        handle_redir(&((*ast)->redir), (*token)->next->value, (*token)->type);
    else if ((*token)->type == HEREDOC)
    {
        handle_redir(&((*ast)->redir), (*token)->next->value, (*token)->type);
    }
    else if ((*token)->type == APPEND)
    {
        handle_redir(&((*ast)->redir), (*token)->next->value, (*token)->type);
    }
    *token = (*token)->next;
    // printf("Current token value: %s\n", (*token)->value);
    return (0);
}

int     parse_pipe(t_token  *token, t_tree_node     **ast)
{
    t_tree_node *new_t_node;

    new_t_node = NULL;
    if (!(*ast))
        return (1);

    init_tree_node(&new_t_node, token);
    new_t_node->left = *ast;
    if ((*ast)->token->type == CMD)
        (*ast)->parent = new_t_node;
    *ast = new_t_node;
    return (0);
}

int parse_word(t_token *token, t_tree_node **ast)
{
    t_tree_node	*new_t_node;
    t_args		*new_arg;

    new_t_node = NULL;
    init_tree_node(&new_t_node, token);
    new_arg = create_arg_node(token->value);
    add_arg(&(new_t_node->token->cmd_args), new_arg);
    if (*ast && (*ast)->token->type == PIPE)
    {
        if ((*ast)->right)
        {
            new_arg = create_arg_node(token->value);
            if (!new_arg)
                return (MALLOC_ERROR);
            add_arg(&((*ast)->right->token->cmd_args), new_arg);
        }
        else
        {
            (*ast)->right = new_t_node;
            new_t_node->parent = *ast;
        }
    }
    else
        *ast = new_t_node;
    return (0);
}

int	parsing(t_token **tokens, t_tree_node **ast, t_ms_var *ms)
{
    t_token *token;
    int     ret;

    ret = 0;
    token = *tokens;
    if (token && token->type == WORD)
        ret = parse_word(token, ast);
    else if (token && token->type == PIPE)
        ret = parse_pipe(token, ast);
    else if (token && (token->type == REDIRECT_IN || token->type == REDIRECT_OUT 
        || token->type == HEREDOC || token->type == APPEND))
        ret = parse_redir(tokens, ast);
    if (token != NULL)
		return (parsing(&(*tokens)->next, ast, ms));
	// create_fds(ast, ms);
    return (ret);
}
