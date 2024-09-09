#include "../minishell.h"

void	add_token(t_token **tokens, char *str, token_type type)
{
    t_token	*new_token;
	t_token	*last;

	if (!type && !str)
		return ;
	new_token = malloc(sizeof(t_token));
    if (!new_token)
        exit (1);
    if (!str)
        new_token->str = NULL;
    else
        new_token->str = str;
    new_token->type = type;
    new_token->next = NULL;
	if (!*tokens)
		*tokens = new_token;
	else
	{
		last = *tokens;
		while (last->next)
			last = last->next;
		last->next = new_token;
	}
}

void print_tokens(t_token *token)
{
	while (token)
    {
        printf("Token: ");
        if (token->str)
            printf("%s", token->str);
		else
			printf("(null)");
		printf(", Type: %d\n", token->type);
        token = token->next;
    }
}

void free_tokens(t_token *token)
{
    t_token *tmp;
    while (token)
	{
        tmp = token;
        token = token->next;
        free(tmp->str);
        free(tmp);
    }
}

char    *get_env(char **input, t_lst *env)
{
	t_var	s;
	char	*env_var;

	s.len = 0;
    (*input)++;
	s.start = *input;
	while (**input && ft_strchr("\'\" \n\t\f\v\r<>|&", **input))
		mv_ptr_incr_len(input, &s.len);
	env_var = ft_substr(s.start, 0, s.len);
	while (ft_strncmp(env->var, env_var, ft_strlen(env_var) + 1))
		env = env->fwd;
    free(env_var);
	env_var = ft_strdup(env->str);
	// (*input)++;
	return (env_var);
}

void    mv_ptr_incr_len(char **input, int *len)
{
	(*input)++;
    (*len)++;
}

bool	valid_quote_pairs(char *input)
{
	char	*sngl_quote_ptr;
	char	*dbl_quote_ptr;

	sngl_quote_ptr = ft_strchr(input, '\'');
	dbl_quote_ptr = ft_strchr(input, '\"');
	
	input = ft
}
