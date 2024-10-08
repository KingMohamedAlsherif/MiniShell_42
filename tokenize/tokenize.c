/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingmohamedalsherif <kingmohamedalsherif@s +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:27:19 by chon              #+#    #+#             */
/*   Updated: 2024/09/15 08:05:14 by kingmohamedalshe ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	set_vars(t_var *s, char **input, char *quote)
{
	s->str = NULL;
	s->std_blockers = "\'\" \n\t\f\v\r<>|&$";
	s->std_block_ex_quotes = " \n\t\f\v\r<>|&$";
	s->sgl_quote_block = "\'";
	s->dbl_quote_block = "\"$";
	s->usd_blockers = "\'\" \n\t\f\v\r<>|&$";
	s->blockers = s->std_blockers;
	s->len = 0;
	s->convert = 0;
	if (**input == '\'')
		s->blockers = s->sgl_quote_block;
	else if (**input == '\"' || *quote == '\"')
		s->blockers = s->dbl_quote_block;
	if (**input == '\'' || **input == '\"')
	{
		*quote = **input;
		(*input)++;
		if (**input == *(*input - 1))
		{
			(*input)++;
			return (1);
		}
	}
	return (1);
}

char    *get_str(char **input, t_lst *env, char quote)
{
	t_var	s;

	if (!set_vars(&s, input, &quote))
		return (NULL);
	if (**input == '$' && quote != '\'')
	{
		(*input)++;
		s.blockers = s.usd_blockers;
		s.convert = 1;
	}
	s.str = get_substr(input, s.blockers);
	if (s.convert)
		s.str = get_env(s.str, env);
	if (quote && **input == quote)
	{
		quote = 0;
		(*input)++;
	}
	if (!**input || (!quote && ft_strchr(s.std_block_ex_quotes, **input)))
		return (s.str);
	return (ft_strjoin(s.str, get_str(input, env, quote)));
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
			exit (1);
		return (PIPE);
	}
}

void    tokenize(char *input, t_token **tokens, t_lst *env) 
{
	*tokens = NULL;

	if (!valid_quote_pairs(input))
		// ft_print_error(*tokens, OPEN_Q);
		printf("hi\n");
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
