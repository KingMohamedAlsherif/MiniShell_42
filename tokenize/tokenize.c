/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:27:19 by chon              #+#    #+#             */
/*   Updated: 2024/09/09 18:35:22 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_str_in_quotes(char **input, char quote, t_lst *env, t_var *s)
{
	char	*str;

	(*input)++;
	// printf("%s\n", *input);
	// printf("%c\n", quote);
	if (**input == quote)
	{
		(*input)++;
		return (NULL);
	}
	s->start = *input;
	while (**input && **input != quote)
	{
		if (quote == '\'')
			while (**input && **input != quote)
				mv_ptr_incr_len(input, &s->len);
		else 
			while (**input && **input != quote && **input != '$')
				mv_ptr_incr_len(input, &s->len);
		s->tmp_str = ft_strjoin(s->tmp_str, ft_substr(s->start, 0, s->len));
		if (**input == '$' && quote == '\"')
			s->tmp_str = ft_strjoin(s->tmp_str, get_env(input, env, quote));
		// (*input)++;
		// if (**input && **input != quote)
		// {
		// 	printf("recursive hits\n");
		// 	(*input)--;
		// 	str = ft_strjoin(str, get_str_in_quotes(input, quote, env, s));
		// }
		str = ft_strjoin(str, s->tmp_str);
		if (**input == quote)
			break ;
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
		if (**input == '\'' || **input == '\"')
			s.tmp_str = get_str_in_quotes(input, **input, env, &s);
		else
		{
			s.start = *input;
			if (*s.start == '$')
				s.tmp_str = get_env(input, env, 1);
			else
			{
				while (**input && !ft_strchr("\'\" \n\t\f\v\r<>|&$", **input))
					mv_ptr_incr_len(input, &s.len);
				s.tmp_str = ft_substr(s.start, 0, s.len);
			}
		}
		if (s.tmp_str)
			s.str = ft_strjoin(s.str, s.tmp_str);
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
		printf("must close quotes\n");
	else
		while (*input)
		{
			while(*input && ft_strchr(" \n\t\f\v\r", *input))
				input++;
			if (!*input)
				break ;
			if (ft_strchr("<>|&", *input))
			{
				add_token(tokens, NULL, get_operator(&input));
				input++;
			}
			else
				add_token(tokens, get_str(&input, env), WORD);
	    }
}
