#include "../minishell.h"

int         parsing(t_token     **tokens, t_tree_node   **AST)
{
    t_token *token;
    int     ret;

    ret = 0;
    token = *tokens;
    if (token->type == WORD)
        ret = parse_word(token, AST);
    else if (token->type == PIPE)
        ret = parse_pip(token, AST);
    if (token != NULL)
        return (parsing(&token->next, AST));
    return (ret);
}

int     parse_word(t_token  *token, t_tree_node **AST)
{
    t_tree_node *new_tree;
    t_args *new_arg;

    new_tree = NULL;
    new_arg = create_arg_node(token->value);
    if (!new_arg)
        return (MALLOC_ERROR);
    if ((*AST)->type == WORD)
    {
        add_arg((*AST)->token->cmd_args, new_arg);
        return (0);
    }
    new_tree = malloc(sizeof(t_tree_node));
    if (!new_tree)
        return (MALLOC_ERROR);
    new_tree->parent = NULL;
    new_tree->token->type = CMD;
    new_tree->token = token;
    new_tree->left = NULL;
    new_tree->right = NULL;
    new_arg = create_arg_node(token->value);
    if (!new_arg)
        return (MALLOC_ERROR);
    add_arg(&(new_tree->token->cmd_args), new_arg);
    if ((*AST)->type == PIPE)
    {
        // new_tree->parent = (*AST);
        (*AST)->right = new_tree;
    }
    return (0);
}

int     parse_pip(t_token  *token, t_tree_node     **AST)
{
    t_tree_node *new_tree;

    if (!(*AST))
        return(SYNTAX_ERROR);
    new_tree = malloc(sizeof(t_tree_node));
    if (!new_tree)
        return (MALLOC_ERROR);
    if ((*AST)->type == WORD)
    {
        new_tree->parent = NULL;
        new_tree->token = token;
        new_tree->type = PIPE;
        new_tree->left = *AST;
        new_tree->right = NULL;
        (*AST)->parent = new_tree;
        *AST = new_tree;
    }
    return (0);
}