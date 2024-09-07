#include "../minishell.h"

// Create a t_token node contain the token with the type
void    add_token(t_token *token_ptr, char *str, token_type type)
{
    token_ptr = malloc(sizeof(t_token));
    if (!token_ptr)
        return (NULL);
    if (!str)
        token_ptr->str = NULL;
    else
        token_ptr->str = ft_strdup(str);
    token_ptr->type = type;
    token_ptr->next = NULL;
    token_ptr = token_ptr->next;
}

// Check and return the token_type
token_type      get_token_type(char *str)
{
    if (str[0] == '|' && str[1] == '\0')
        return(SPACE);
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
    char   *str;
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
            str = get_next_token(input);
            if (getenv(str) == NULL)
                return(str);
            len = ft_strlen(getenv(str));
            word = malloc(len);
            ft_strlcpy(word, getenv(str), len);
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

int	get_operator(char **input)
{
	if (**(input + 1) && **(input + 1) == **input)
	{
		(*input)++;
		if (**input == '<')
			return (HEREDOC);
		if (**input == '>')
			return (APPEND_OUT);
		if (**input == '|')
			return (OR);
		else
			return (AND);
	}
	else
	{
		if (**input == '<')
			return (REDIRECT_IN);
		if (**input == '>')
			return (REDIRECT_OUT);
		if (**input == '|')
			return (PIPE);
	}
}

char    *get_str(char **input)
{
	char	*str;
	int		len;
	char	quote;
	int		quote_switch;

	len = 0;
	quote_switch = 0;
	while (!ft_strchr(" \n\t\f\v\r", **input))
	{
		if (*input == '\'' || *input == '\"')
		{
			quote = *input;
			quote_switch = (quote_switch + 1) % 2;
			(*input)++;
		}
		if (*input != quote)
			len++;
		(*input)++;
	}
}

void    tokenize(char *input, t_token **tokens) 
{
    t_token *token_ptr;

    *tokens = NULL;
    token_ptr = *tokens;
    while (*input)
    {
        while(ft_strchr(" \n\t\f\v\r", *input))
            input++;
        else if (ft_strchr("<>|&", *input))
			add_token(token_ptr, NULL, get_operator(&input));
        else
            add_token(token_ptr, get_str(&input), WORD);
		input++;
    }
}

void print_tokens(t_token *tokens) {
    while (tokens) 
    {
        printf("Token: %s, Type: %d\n", tokens->str, tokens->type);
        tokens = tokens->next;
    }
}

// Function to free the tokens
void free_tokens(t_token *tokens) {
    t_token *tmp;
    while (tokens) {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->str);
        free(tmp);
    }
}

// void        handle_env(t_token  **tokens)
// {
//     t_token *tmp;

//     tmp = *tokens;
//     while(tmp)
//     {
//         if (tmp->str[0] == '$')
//         {
//             tmp->str = &tmp->str[1];
//             tmp->str = getenv(tmp->str);
//             printf("This is env: %s\n", tmp->str);
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
