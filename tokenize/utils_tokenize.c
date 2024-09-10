/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:27:11 by chon              #+#    #+#             */
/*   Updated: 2024/09/10 15:00:57 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_token(t_token **tokens, char *str, token_type type)
{
	t_token	*new_token;
	t_token	*last;

	if (type < 0 || (type == WORD && !str))
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

void	print_tokens(t_token *token)
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

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->str);
		free(tmp);
	}
}

// char	*get_env(char **input, t_lst *env, char	convert)
// {
// 	t_var	s;
// 	char	*env_val;

// 	s.len = 0;
// 	(*input)++;
// 	if (!**input || ft_strchr("\'\" $", **input))
// 		return (ft_strdup("$"));
// 	s.start = *input;
// 	while (**input && !ft_strchr("\'\" \n\t\f\v\r<>|&$", **input))
// 		mv_ptr_incr_len(input, &s.len);
// 	env_val = ft_substr(s.start, 0, s.len);
// 	if (convert == 1 || convert == '\"')
// 	{
// 		while (env && ft_strncmp(env->var, env_val, ft_strlen(env_val)))
// 			env = env->fwd;
// 		free(env_val);
// 		if (!env)
// 			return (NULL);
// 		env_val = ft_strdup(env->val);
// 	}
// 	// (*input)++;
// 	return (env_val);
// }

char	*get_env_test(char **input, t_lst *env)
{
	t_var	s;
	char	*env_val;

	s.len = 0;
	(*input)++;
	if (!**input || ft_strchr("\'\" $", **input))
	{
		if (**input)
			(*input)++;
		return (ft_strdup("$"));
	}
	while (env && ft_strncmp(env->var, env_val, ft_strlen(env_val)))
		env = env->fwd;
	if (!env)
		return (NULL);
	env_val = ft_strdup(env->val);
	return (env_val);
}

void	mv_ptr_incr_len(char **input, int *len)
{
	(*input)++;
	(*len)++;
}

bool	valid_quote_pairs(char *input)
{
	while (*input)
	{
		if (*input == '\'' || *input == '\"')
		{
			if (!(input + 1))
				return (0);
			input = ft_strchr(input + 1, *input);
			if (!input)
				return (0);
			input++;
		}
		else
			while (*input && (*input != '\'' || *input != '\"'))
				input++;
	}
	return (1);
}
