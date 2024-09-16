/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingmohamedalsherif <kingmohamedalsherif@s +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:27:11 by chon              #+#    #+#             */
/*   Updated: 2024/09/15 10:41:18 by kingmohamedalshe ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void add_token(t_token **tokens, char *str, token_type type)
{
	t_token *new_token;
	t_token *last;

	// printf("%s\n", str);
	// if (type < 0 || (type == WORD && !str))
	// if (type < 0)
	// 	return ;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		exit(1);
	if (!str)
		new_token->value = ft_strdup("\0");
	else
		new_token->value = str;
	new_token->type = type;
	new_token->next = NULL;
	new_token->exec_cmd_path = NULL;
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

void print_tokens(t_token *token)
{
	while (token)
	{
		printf("Token: ");
		if (token->value)
			printf("%s", token->value);
		else
			printf("(null)");
		printf(", Type: %d\n", token->type);
		token = token->next;
	}
}

void free_tokens(t_token *token)
{
	t_token *tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->value);
		free(tmp);
	}
}

char *get_env(char *str, t_lst *env)
{
	size_t	str_len;

	str_len = ft_strlen(str);
	if (!str || !str_len)
		return (ft_strdup("$"));
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
void		ft_print_error(t_token	*tokens, token_type	type)
{
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
	else if (type == OPEN_Q)
		write(2, "syntax error 'uncloused qouts'\n", 32);
	free_tokens(tokens);
}

void check_syntax(t_token *tokens_list)
{
	t_token *token;
	t_token *next;

	if (!tokens_list)
		return;
	token = tokens_list;
	if (token->type == PIPE || token->type == OR || token->type == AND)
		return (ft_print_error(token, token->type));
	while (token)
	{
		next = token->next; 
		if ((token->type == REDIRECT_IN || token->type == REDIRECT_OUT ||
			 token->type == APPEND || token->type == HEREDOC) &&
			(!next || next->type != WORD))
			return (ft_print_error(token, token->type));
		if ((token->type == PIPE || token->type == OR || token->type == AND) && !next)
			return (ft_print_error(token, token->type));
		token = token->next;
	}
}
