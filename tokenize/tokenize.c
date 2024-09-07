#include "../minishell.h"

char    *get_str(char **input, t_lst *env)
{
	t_var	s;

	s.tmp_str = NULL;
	s.str = NULL;
	s.start = *input;
	while (**input && ft_strchr(" \n\t\f\v\r<>|&", **input))
	{
		if (ft_strchr("\'\"", **input))
		{
			s.quote = **input;
			(*input)++;
			s.len = 0;
			if (s.quote == '\'')
			{
				while (**input && **input != s.quote)
					mv_ptr_incr_len(input, &s.len);
				s.tmp_str = ft_substr(s.start, 0, s.len);
			}
			else
			{
				while (**input && **input != s.quote && **input != '$')
					mv_ptr_incr_len(input, &s.len);
				s.tmp_str = ft_substr(s.start, 0, s.len);
				if (**input == '$')
					s.str = ft_strjoin(s.tmp_str, get_env(input, env));
			}
		}
		else
		{
			s.len = 0;
			while (**input && ft_strchr("\'\" \n\t\f\v\r<>|&", **input))
				mv_ptr_incr_len(input, &s.len);
			if (*s.start == '$')
				s.tmp_str = get_env(input);
			else
				s.tmp_str = ft_substr(s.start, 0, s.len);
		}
	}
	if (!s.str)
		s.str = s.tmp_str;
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
