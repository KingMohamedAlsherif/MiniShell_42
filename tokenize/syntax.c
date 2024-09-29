/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:27:19 by chon              #+#    #+#             */
/*   Updated: 2024/09/30 02:07:33 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_syntax_error(t_token *tokens, t_token_type type)
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
		tokens = tokens->next;
		free(tmp_token->value);
		free(tmp_token);
	}
}

bool	syntax_errors(t_token *tokens)
{
	t_token	*tokens_ptr;

	tokens_ptr = tokens;
	if (tokens->type == PIPE)
		return (print_syntax_error(tokens_ptr, tokens->type), 1);
	while (tokens)
	{
		if ((tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT
				|| tokens->type == APPEND || tokens->type == HEREDOC)
			&& (!tokens->next || tokens->next->type != WORD))
			return (print_syntax_error(tokens_ptr, tokens->type), 1);
		if ((tokens->type == PIPE) && (!tokens->next))
			return (print_syntax_error(tokens_ptr, tokens->type), 1);
		tokens = tokens->next;
	}
	return (0);
}
