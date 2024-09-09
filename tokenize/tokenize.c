#include "../minishell.h"

char	*get_str_in_quotes(char **input, char quote, t_lst *env, t_var *s)
{
	char	*str;

	(*input)++;
	printf("%s\n", *input);
	// printf("%c\n", quote);
	if (**input == quote)
	{
		(*input)++;
		return (NULL);
	}
	s->start = *input;
	if (!ft_strchr(*input, quote))
	{
		printf("exit hit\n");
		exit (1);
	}
	while (**input && **input != quote)
	{
		if (quote == '\'')
			while (**input && **input != quote)
				mv_ptr_incr_len(input, &s->len);
		else 
			while (**input && **input != quote && **input != '$')
				mv_ptr_incr_len(input, &s->len);
		str = ft_substr(s->start, 0, s->len);
		(*input)++;
		if (**input == '$')
			str = ft_strjoin(str, get_env(input, env));
		if (**input && **input != quote)
		{
			printf("recursive hits\n");
			(*input)--;
			str = ft_strjoin(str, get_str_in_quotes(input, quote, env, s));
		}
	}
	return (str);
}

char    *get_str(char **input, t_lst *env)
{
	t_var	s;

	s.str = NULL;
	while (**input && !ft_strchr(" \n\t\f\v\r<>|&", **input))
	{
		s.tmp_str = NULL;
		s.len = 0;
		if (ft_strchr("\'\"", **input))
			s.tmp_str = get_str_in_quotes(input, **input, env, &s);
		else
		{
			s.start = *input;
			while (**input && !ft_strchr("\'\" \n\t\f\v\r<>|&", **input))
				mv_ptr_incr_len(input, &s.len);
			if (*s.start == '$')
				s.tmp_str = get_env(input, env);
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
	if (*(*input + 1) && *(*input + 1) == **input)
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
		if (**input == '&')
			exit (1);
		return (PIPE);
	}
}

void    tokenize(char *input, t_token **tokens, t_lst *env) 
{
	*tokens = NULL;

	if (!valid_quote_pairs(input))
		exit (1);
	while (*input)
    {
		while(ft_strchr(" \n\t\f\v\r", *input))
            input++;
        if (ft_strchr("<>|&", *input))
		{
			add_token(tokens, NULL, get_operator(&input));
			input++;
		}
        else
            add_token(tokens, get_str(&input, env), WORD);
    }
}
