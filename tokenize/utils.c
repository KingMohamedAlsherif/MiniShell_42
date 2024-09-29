/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:27:11 by chon              #+#    #+#             */
/*   Updated: 2024/09/30 02:05:09 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	get_cmd(t_token **tokens, t_lst *env)
{
	char	*line;
	char	*line_ptr;
	int		start;

	line = readline("> ");
	line_ptr = line;
	start = 0;
	while (line_ptr[start] && ft_strchr(" \n\t\f\v\r", line_ptr[start]))
		start++;
	line = ft_substr(line_ptr, start, ft_strlen(line));
	free(line_ptr);
	line_ptr = line;
	if (!valid_quote_pairs(line_ptr))
		printf("Must input closing quote\n");
	else
		add_token(tokens, get_str(&line, env, 0), WORD);
	free(line_ptr);
}

void	add_token(t_token **tokens, char *str, t_token_type type)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		exit(1);
	new_token->value = str;
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

char	*get_env(char *str, t_lst *env)
{
	size_t	str_len;

	str_len = ft_strlen(str);
	if (!str || !str_len)
	{
		free(str);
		return (ft_strdup("$"));
	}
	while (env)
	{
		if (ft_strlen(env->var) > str_len)
			str_len = ft_strlen(env->var);
		if (!ft_strncmp(env->var, str, str_len))
			break ;
		env = env->fwd;
	}
	free(str);
	if (!env)
		return (NULL);
	return (ft_strdup(env->val));
}

char	*get_substr(char **input, char *blocker)
{
	int		len;
	char	*start;

	if (!**input)
		return (NULL);
	len = 0;
	start = *input;
	while (**input && !ft_strchr(blocker, **input))
	{
		(*input)++;
		len++;
	}
	return (ft_substr(start, 0, len));
}

t_token	*last_token(t_token *token)
{
	while (token->next)
		token = token->next;
	return (token);
}
