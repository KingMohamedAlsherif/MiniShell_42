/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:27:19 by chon              #+#    #+#             */
/*   Updated: 2024/09/11 14:30:02 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// char	*get_str_in_quotes(char **input, char quote, t_lst *env, t_var *s)
// {
// 	char	*str;

// 	(*input)++;
// 	// printf("%s\n", *input);
// 	// printf("quote: %c\n", quote);
// 	if (**input == quote)
// 	{
// 		(*input)++;
// 		return (NULL);
// 	}
// 	s->start = *input;
// 	str = NULL;
// 	while (**input && **input != quote)
// 	{
// 		s->tmp_str = NULL;
// 		s->len = 0;
// 		printf("**input: %c\n", **input);
// 		if (quote == '\'')
// 		{
// 			while (**input && **input != quote)
// 				mv_ptr_incr_len(input, &s->len);
// 			s->tmp_str = ft_substr(s->start, 0, s->len);
// 		}
// 		else
// 		{
// 			while (**input && **input != quote && **input != '$')
// 				mv_ptr_incr_len(input, &s->len);
// 			s->tmp_str = ft_substr(s->start, 0, s->len);
// 			if (**input == '$' && quote == '\"')
// 				s->tmp_str = ft_strjoin(s->tmp_str, get_env(input, env, quote));
// 		}
// 		// if (**input && **input != quote)
// 		// {
// 		// 	printf("recursive hits\n");
// 		// 	(*input)--;
// 		// 	str = ft_strjoin(str, get_str_in_quotes(input, quote, env, s));
// 		// }
// 		// printf("tmpstr: %s\n", s->tmp_str);
// 		str = ft_strjoin(str, s->tmp_str);
// 		if (**input == quote)
// 		{
// 			(*input)++;
// 			break ;
// 		}
// 	}
// 	return (str);
// }

// char    *get_str(char **input, t_lst *env)
// {
// 	t_var	s;

// 	s.str = NULL;
// 	while (**input && !ft_strchr(" \n\t\f\v\r<>|&", **input))
// 	{
// 		s.tmp_str = NULL;
// 		s.len = 0;
// 		if (**input == '\'' || **input == '\"')
// 			s.tmp_str = get_str_in_quotes(input, **input, env, &s);
// 		else
// 		{
// 			s.start = *input;
// 			if (*s.start == '$')
// 				s.tmp_str = get_env(input, env, 1);
// 			else
// 			{
// 				while (**input && !ft_strchr("\'\" \n\t\f\v\r<>|&$", **input))
// 					mv_ptr_incr_len(input, &s.len);
// 				s.tmp_str = ft_substr(s.start, 0, s.len);
// 			}
// 		}
// 		s.str = ft_strjoin(s.str, s.tmp_str);
// 	}
// 	return (s.str);
// }

char    *get_str(char **input, t_lst *env, char quote)
{
	t_var	s;

	s.str = NULL;
	s.std_blockers = ft_strdup("\'\" \n\t\f\v\r<>|&$");
	s.std_block_ex_quotes = ft_strdup(" \n\t\f\v\r<>|&$");
	s.sgl_quote_block = ft_strdup("\'");
	s.dbl_quote_block = ft_strdup("\"$");
	s.blockers = s.std_blockers;
	while (**input)
	{
		s.len = 0;
		s.convert = 0;
		if (**input == '\'')
			s.blockers = s.sgl_quote_block;
		else if (**input == '\"')
			s.blockers = s.dbl_quote_block;
		if (**input == '\'' || **input == '\"')
		{
			quote = **input;
			(*input)++;
			if (**input == *(*input - 1))
			{
				(*input)++;
				return (NULL);
			}
		}
		if (**input == '$' && quote != '\'')
		{
			(*input)++;
			s.blockers = s.std_blockers;
			s.convert = 1;
		}
		s.start = *input;
		s.str = get_substr(input, s.blockers);
		if (s.convert)
			s.str = get_env_test(s.str, env);
		// printf("%c\n", **input);
		if (!**input || (!quote && ft_strchr(s.std_block_ex_quotes, **input))
			|| (quote && **input == quote
				&& ft_strchr(s.std_block_ex_quotes, *(*input + 1))))
		{
			if (quote && **input == quote)
				(*input)++;	
			break ;
		}
		s.str = ft_strjoin(s.str, get_str(input, env, 0));
	}
	// printf("%s\n", s.str);
	return (s.str);
}

int	get_operator(char **input)
{
	(*input)++;
	if (**input && **input == *(*input - 1))
	{
		(*input)++;
		if (*(*input - 1) == '<')
			return (HEREDOC);
		if (*(*input - 1) == '>')
			return (APPEND_OUT);
		if (*(*input - 1) == '|')
			return (OR);
		else
			return (AND);
	}
	else
	{
		if (*(*input - 1) == '<')
			return (REDIRECT_IN);
		if (*(*input - 1) == '>')
			return (REDIRECT_OUT);
		if (*(*input - 1) == '&')
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
				add_token(tokens, NULL, get_operator(&input));
			else
				add_token(tokens, get_str(&input, env, 0), WORD);
	    }
}
