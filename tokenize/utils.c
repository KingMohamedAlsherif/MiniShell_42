/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:27:11 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 00:11:08 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void add_token(t_token **tokens, char *str, token_type type)
{
	t_token *new_token;
	t_token *last;

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

char *get_env(char *str, t_lst *env)
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

char *get_substr(char **input, char *blocker)
{
	int len;
	char *start;

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

void	print_syntax_error(t_token *tokens, token_type type)
{
	t_token	*tmp_token;

	if (type == PIPE)
		write(2, "syntax error near unexpected token `|'\n", 40);
	else if (type == REDIRECT_IN)
		write(2, "syntax error near unexpected token `<'\n", 40);
	else if (type == REDIRECT_OUT)
		write(2, "syntax error near unexpected token `>'\n", 40);
	else if (type == HEREDOC)
		write(2, "syntax error near unexpected token `<<'\n", 41);
	else if (type == APPEND)
		write(2, "syntax error near unexpected token `>>'\n", 41);
	while (tokens)
	{
		tmp_token = tokens;
		// printf("%d: %s\n", tmp_token->type, tmp_token->value);
		tokens = tokens->next;
		free(tmp_token->value);
		free(tmp_token);
	}
}

bool	syntax_errors(t_token *tokens)
{
	t_token *tokens_ptr;

	tokens_ptr = tokens;
	if (tokens->type == PIPE || tokens->type == OR || tokens->type == AND)
		return (print_syntax_error(tokens_ptr, tokens->type), 1);
	while (tokens)
	{
		if ((tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT ||
			 tokens->type == APPEND || tokens->type == HEREDOC) &&
			(!tokens->next || tokens->next->type != WORD))
			return (print_syntax_error(tokens_ptr, tokens->type), 1);
		if ((tokens->type == PIPE || tokens->type == OR || tokens->type == AND) 
		&& (!tokens->next || tokens->next->type == END))
			return (print_syntax_error(tokens_ptr, tokens->type), 1);
		tokens = tokens->next;
	}
	return (0);
}
