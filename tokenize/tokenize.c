/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:27:19 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 11:33:51 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_vars(t_var *s, char **input, char *quote)
{
	s->str = NULL;
	s->std_blockers = "\'\" \n\t\f\v\r<>|&$";
	s->std_block_ex_quotes = " \n\t\f\v\r<>|&";
	s->sgl_quote_block = "\'";
	s->dbl_quote_block = "\"$";
	s->usd_blockers = "\'\" \n\t\f\v\r<>|&$";
	s->blockers = s->std_blockers;
	s->len = 0;
	s->convert = 0;
	if (**input == '\'' && !*quote)
		s->blockers = s->sgl_quote_block;
	else if (**input == '\"' || *quote == '\"')
		s->blockers = s->dbl_quote_block;
	if (**input == '\'' || **input == '\"')
		if (!*quote)
		{
			*quote = **input;
			(*input)++;
		}
}

char    *get_str(char **input, t_lst *env, char quote)
{
	t_var	s;

	set_vars(&s, input, &quote);
	if (**input == '$' && quote != '\'')
	{
		(*input)++;
		s.blockers = s.usd_blockers;
		s.convert = 1;
	}
	s.str = get_substr(input, s.blockers);
	if (s.convert)
		s.str = get_env(s.str, env);
	if (!s.str || !*s.str)
	{
		free(s.str);
		return (NULL);
	}
	if (quote && **input == quote)
	{
		quote = 0;
		(*input)++;
	}
	if (!**input || (!quote && ft_strchr(s.std_block_ex_quotes, **input)))
		return (s.str);
	return (ft_strjoin(s.str, get_str(input, env, quote), 1, 1));
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
			return (APPEND);
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
			return (AND);
		return (PIPE);
	}
}

bool valid_quote_pairs(char *input)
{
	while (*input)
	{
		if (*input == '\'' || *input == '\"')
		{
			if (!*(input + 1))
				return (0);
			input = ft_strchr(input + 1, *input);
			if (!input)
				return (0);
			input++;
		}
		else
			while (*input && !(*input == '\'' || *input == '\"'))
				input++;
	}
	return (1);
}

void    tokenize(char *input, t_token **tokens, t_lst *env) 
{
	char *str;

	str = NULL;
	*tokens = NULL;
	if (!valid_quote_pairs(input))
		printf("Must input closing quote\n");
	else
	{
		while (*input)
		{
			// printf("%c: %d\n", *input, (*tokens)->type);
			while(*input && ft_strchr(" \n\t\f\v\r", *input))
				input++;
			if (!*input)
				break ;
			if (ft_strchr("<>|&", *input))
				add_token(tokens, NULL, get_operator(&input));
			else
			{
				str = get_str(&input, env, 0);
				if (str) // Only if str not NULL
					add_token(tokens, str, WORD);
			}
			if (is_empty(input) && last_token((*tokens))->type == PIPE)
				get_cmd(tokens, env);
		}
		add_token(tokens, NULL, END);
	}
}
