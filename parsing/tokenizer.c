#include "../minishell.h"

// Creat a t_token node contain the token with the type
t_token *create_token(char *value, token_type type) {
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
        return NULL;
    new_token->value = ft_strdup(value);
    new_token->type = type;
    new_token->next = NULL;
    new_token->cmd_args = NULL;
    return new_token;
}

// Add the token to the list of tokens
void add_token(t_token **tokens, t_token *new_token) {
    if (*tokens == NULL) {
        *tokens = new_token;
    } else {
        t_token *current = *tokens;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
}

// Check and return the token_type
token_type      get_token_type(char *str)
{
    if (str[0] == '|' && str[1] == '\0')
        return(PIPE);
    else if (str[0] == '|' && str[1] == '\0')
        return (PIPE);
    else if (str[0] == '<' && str[1] == '\0')
        return (REDIRECT_IN);
    else if (str[0] == '<' && str[1] == str[0])
        return (HEREDOC);
    else if (str[0] == '>' && str[1] == '\0')
        return (REDIRECT_OUT);
    else if (str[0] == '"')
        return (DOUBLE_Q);
    else if (str[0] == '\'')
        return (SINGLE_Q);
    else if (str[0] == '|' && str[1] == str[0])
        return (OR);
    else if (str[0] == '&' && str[1] == str[0])
        return (AND);
    else
        return (WORD);
}

int         search_closed_qouts(char   **input)
{
    char c;
    c = **input;
    (*input)++;
    while (*input)
    {
        if (**input == c)
            return (0);
        (*input)++;
    }
    return (UNCLOUSED_Q); //  2 on  uncloused qouts
}
// get the next token by iterating inside the input line
char *get_next_token(char **input)
{
    char *start;
    char   *value;
    char *word;
    // char quote = 0;
    size_t len = 0;

    // Skip leading spaces
    while (**input && isspace(**input))
        (*input)++;
    start = *input;
    if (**input == '\'' || **input == '"')
    {
        if (search_closed_qouts(input) == 1)
            return (NULL);
        if (start[len] == '\'' && start[len + 1] != '\0') {
            len++;
            while(start[++len] != '\'')
            {
                if (start[len] == '$')
                {
                    *input = &start[len];
                    get_next_token(input);
                }
            }
            *input = &start[len + 1];
        }
        else {
            (*input)++;
            len = *input - start;
        }
    }
    else
    {
        if (**input == '$') {
            (*input)++;
            value = get_next_token(input);
            if (getenv(value) == NULL)
                return(value);
            len = ft_strlen(getenv(value));
            word = malloc(len);
            ft_strlcpy(word, getenv(value), len);
            word[len] = '\0';
            return word;
            }
        if (**input == '\0' || **input == '$')
            exit(1);
        while (**input)
        {
            if ((isspace(**input) || ft_strchr("|<>$", **input))) 
                break;
            (*input)++;
            len++;
        }
    }
    word = malloc(len + 1);
    if (!word)
        return (NULL); // return 1 when malloc error
    strncpy(word, start, len);
    word[len] = '\0';
    return word;
}

int skip_whitespace(char **input) 
{
    if (**input == '\0' || !isspace(**input))
        return (SYNTAX_ERROR);
    while (**input && isspace(**input))
        (*input)++;
    return(0);
}

t_token *tokenize(char **input, t_token  **tokens) 
{
    char operator[3];

    if (!input || !*input) // Check input is NULL
        return NULL;
    while (**input)
    {
        // if (skip_whitespace(input) == 0)
        // {
        //     // char *word = get_next_token(&input);
        //     add_token(tokens, create_token("I'm Space",SPACE));
        // }
        if (**input == '\0')
            break;
        if (ft_strchr("|<>", **input)) 
        {
            char operator[3] = { **input, '\0', '\0'};
            if ((**input == '>' || **input == '<' || **input == '|' || **input == '&') && *(*input + 1) == **input)
            {
                operator[1] = **input;
                (*input)++;
            }
            add_token(tokens, create_token(operator, get_token_type(operator)));
            (*input)++;
        }
        else if (ft_strchr("$", **input))
        {
            char *word = get_next_token(input);
            add_token(tokens, create_token(word, get_token_type(word)));
            free(word);
        }
        else
        {
            char *word = get_next_token(input);
            add_token(tokens, create_token(word,get_token_type(word)));
            free(word);
        }
    }
    return (0);
}

void print_tokens(t_token *tokens) {
    while (tokens) 
    {
        printf("Token: %s, Type: %d\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

// Function to free the tokens
void free_tokens(t_token *tokens) {
    t_token *tmp;
    while (tokens) {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->value);
        free(tmp);
    }
}

// void        handle_env(t_token  **tokens)
// {
//     t_token *tmp;

//     tmp = *tokens;
//     while(tmp)
//     {
//         if (tmp->value[0] == '$')
//         {
//             tmp->value = &tmp->value[1];
//             tmp->value = getenv(tmp->value);
//             printf("This is env: %s\n", tmp->value);
//         }
//         tmp = tmp->next;
//     }
// }
// Example usage
// int main(void) 
// {
//     char *input = "echo              echo$USER  'hello$USER' | wc";
//     int j = 1;
//     t_token *tokens;
//     tokenize(input, tokens);
//     printf("Try getenv: %s\n", getenv("USER"));
//     // handle_env(&tokens);
//     print_tokens(tokens);
//     return 0;
// }
