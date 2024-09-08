#include "../minishell.h"

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

void print_tokens(t_token *tokens)
{
    while (tokens) 
    {
        printf("Token: %s, Type: %d\n", tokens->str, tokens->type);
        tokens = tokens->next;
    }
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;
    while (tokens)
	{
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->str);
        free(tmp);
    }
}

char    *get_env(char **input, t_lst *env)
{
	t_var	s;
	char	*env_var;

	s.len = 0;
	s.start = *input;
	while (**input && ft_strchr("\'\" \n\t\f\v\r<>|&", **input))
		mv_ptr_incr_len(input, &s.len);
	env_var = ft_substr(s.start, 0, s.len);
	while (ftstrncmp(env->var, env_var, ft_strlen(env_var) + 1))
		env = env->fwd;
    free(env_var);
	env_var = ft_strdup(env->str);
	return (env_var);
}

void    mv_ptr_incr_len(char **input, int *len)
{
    (*input)++;
    (*len)++;
}
