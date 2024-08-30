#include "../minishell.h"

int     check_cmd_and_args(t_token   **tokens)
{
    t_token *token;

    token = *tokens;

    while(token) 
    {
        if (token->type == WORD)
            token->type = CMD;
        else if (token->type == PIPE)
            return (SYNTAX_ERROR);
        if (token->next->type == WORD)
            token->cmd_args = token;
        token = token->next;
    }
    return(0);
}