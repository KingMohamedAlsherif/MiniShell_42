#ifndef MINISHELL_H
#   define MINISHELL_H

// INCLUDE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


// STRUCT
typedef enum {
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    HEREDOC,
}   token_type;

typedef struct s_token 
{
    token_type  type;
    char    *str;
    struct s_token  *next;
}   t_token;


#endif