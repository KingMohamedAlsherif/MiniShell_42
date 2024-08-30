#ifndef PARSING_H
#define PARSING_H
// -----INCLUDE-----
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#define MALLOC_ERROR 1
#define UNCLOUSED_Q 2

// -----STRUCT------
typedef enum {
    WORD,
    DOUBLE_Q,
    SINGLE_Q,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    HEREDOC,
    DOLLAR_S,
    SPACE,
    OR,
    AND
} token_type;


#endif