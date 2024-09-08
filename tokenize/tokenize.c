#include "../minishell.h"

char	*get_str_in_quotes(char **input, char quote, t_lst *env, t_var *s)
{
	char	*str;

	(*input)++;
	s->start = *input;
	while (**input != quote)
	{
		if (quote == '\'')
			while (**input && **input != quote)
				mv_ptr_incr_len(input, &s->len);
		else 
			while (**input && **input != quote && **input != '$')
				mv_ptr_incr_len(input, &s->len);
		if (!(**input))
			exit (1);
		if (*s->start == '$')
			str = get_env(input, env);
		else
			str = ft_substr(s->start, 0, s->len);
		if (**input != quote)
			str = ft_strjoin(str, get_str_in_quotes(input, quote, env, s));
	}
	return (str);
}

char    *get_str(char **input, t_lst *env)
{
	t_var	s;

	s.str = NULL;
	while (**input && ft_strchr(" \n\t\f\v\r<>|&", **input))
	{
		s.tmp_str = NULL;
		s.len = 0;
		if (ft_strchr("\'\"", **input))
			get_str_in_quotes(input, **input, &s);
		else
		{
			s.start = *input;
			while (**input && ft_strchr("\'\" \n\t\f\v\r<>|&", **input))
				mv_ptr_incr_len(input, &s.len);
			if (*s.start == '$')
				s.tmp_str = get_env(input);
			else
				s.tmp_str = ft_substr(s.start, 0, s.len);
		}
		if (s.tmp_str)
			s.str = ft_strjoin(s.tmp_str, s.str);
	}
	return (s.str);
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

void    tokenize(char *input, t_token **tokens, t_lst *env) 
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
            add_token(token_ptr, get_str(&input, env), WORD);
		input++;
    }
}
