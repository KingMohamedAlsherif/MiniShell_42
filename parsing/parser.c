#include "../minishell.h"

int         parsing(t_token     **tokens, t_tree_node   **AST)
{
    t_token *token;
    int     ret;

    ret = 0;
    token = *tokens;
    if (token->type == WORD)
        ret = parse_word(&token, &AST);
    else if (token->type == PIPE)
        ret = parse_pip(&token, &AST);
    if (token != NULL)
        return (parsing(&token->next, &AST));
    return (ret);
}

int     parse_word(t_token  *token, t_tree_node **AST)
{
    t_tree_node *new_tree;

    if (!(*AST))
    {
        
        new_tree = malloc(sizeof(t_tree_node));
        new_tree->parent = token;
        new_tree->value = ft_strdup(token->value);
        new_tree->left = NULL;
        new_tree->right = NULL;
    }
    else
    {
        new_tree->right = token;
    }
}

int     parse_pip(t_token  *token, t_tree_node     **AST)
{
    if(!(*AST))
        return(SYNTAX_ERROR);
    else if ((*AST)->type == WORD)
    {

    }
}