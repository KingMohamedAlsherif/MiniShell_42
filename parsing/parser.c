#include "../minishell.h"

int	parsing(t_token **tokens, t_tree_node **ast, t_lst *ms_env)
{
    t_token *token;
    int     ret;

    ret = 0;
    token = *tokens;
    if (token && token->type == WORD)
        ret = parse_word(token, ast);
    else if (token && token->type == PIPE)
        ret = parse_pip(token, ast);
    else if (token && (token->type == REDIRECT_IN || token->type == REDIRECT_OUT 
    || token->type == HEREDOC || token->type == APPEND))
        ret = parse_redir(tokens, ast);
    if (token != NULL)
        return (parsing(&(*tokens)->next, ast, ms_env));
	create_fds(ast, ms_env);
    return (ret);
}

int      parse_word(t_token  *token, t_tree_node **ast)
{
    t_tree_node *new_tree;
    t_args *new_arg;

    if ((*ast) && (*ast)->type == CMD)
    {
        // printf("ast cmd\n");
        printf("token: %s\n", token->value);
        new_arg = create_arg_node(token->value);
        if (!new_arg)
            return (MALLOC_ERROR);
        add_arg(&((*ast)->token->cmd_args), new_arg);
        return (0);
    }
    new_tree = malloc(sizeof(t_tree_node));
    if (!new_tree)
        return (MALLOC_ERROR);
    new_tree->type = CMD;
    new_tree->token = token;
    new_tree->left = NULL;
    new_tree->right = NULL;
    new_tree->redir = NULL;
    new_arg = create_arg_node(new_tree->token->value);
    if (!new_arg)
        return (MALLOC_ERROR);
    add_arg(&(new_tree->token->cmd_args), new_arg);
    if((*ast) && (*ast)->type == PIPE)
    {
        if ((*ast)->right)
         {
             new_arg = create_arg_node(token->value);
             if (!new_arg)
                 return (MALLOC_ERROR);
             add_arg(&((*ast)->right->token->cmd_args), new_arg);
             return (0);
         }
        (*ast)->right = new_tree; // cmd to right
        new_tree->parent = *ast;  // pipe as the parent
    }
    else
        *ast = new_tree;
    return (0);
}



int     parse_pip(t_token  *token, t_tree_node     **ast)
{
    t_tree_node *new_tree;

    if (!(*ast))
    {
        printf("SYNTAX_ERROR\n");
        return (SYNTAX_ERROR);
    }
    new_tree = malloc(sizeof(t_tree_node));
    if (!new_tree)
    {
        printf("MALLOC_ERROR\n");
        return (MALLOC_ERROR);
    }
    new_tree->token = token;
    new_tree->type = PIPE;
    new_tree->parent = NULL;
    new_tree->left = *ast;
    new_tree->right = NULL;
    new_tree->redir = NULL;
    if ((*ast)->type == CMD)
        (*ast)->parent = new_tree;
    *ast = new_tree;
    return (0);
}

int         parse_redir(t_token     **token, t_tree_node     **ast)
{
    if (!token || !*token || !ast || !*ast || !(*token)->next) // ensure that next token is word
        return (SYNTAX_ERROR);
    if ((*token)->type == REDIRECT_IN)
        redir_in(&((*ast)->redir), (*token)->next->value);
    else if ((*token)->type == REDIRECT_OUT)
        redir_out(&((*ast)->redir), (*token)->next->value);
    else if ((*token)->type == HEREDOC)
    {
        redir_heredoc(&((*ast)->redir), (*token)->next->value);
    }
    else if ((*token)->type == APPEND)
    {
        redir_append(&((*ast)->redir), (*token)->next->value);
    }
    *token = (*token)->next;
    // printf("Current token value: %s\n", (*token)->value);
    return (0);
}