/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 04:25:38 by malsheri          #+#    #+#             */
/*   Updated: 2024/09/30 01:36:52 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_node(t_redir *new_redir, char *value, t_token_type type)
{
	new_redir->filename = ft_strdup(value);
	if (type == HEREDOC)
		new_redir->heredoc_delim = ft_strdup(value);
	else if (type == APPEND)
		new_redir->is_append = 1;
	else if (type == REDIRECT_IN)
		new_redir->in_fd = 1;
	else if (type == REDIRECT_OUT)
		new_redir->out_fd = 1;
}

void	parse_redir(t_token *token, t_redir **redir)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->filename = NULL;
	new_redir->heredoc_delim = NULL;
	new_redir->in_fd = 0;
	new_redir->is_append = 0;
	new_redir->out_fd = 0;
	new_redir->fwd = NULL;
	new_redir->bwd = NULL;
	if (!*redir)
		*redir = new_redir;
	else
	{
		while ((*redir)->fwd)
			*redir = (*redir)->fwd;
		(*redir)->fwd = new_redir;
		new_redir->bwd = *redir;
	}
	update_node(new_redir, token->next->value, token->type);
}

void	parse_word(t_token *token, t_tree_node **n, t_ms_var *ms)
{
	if (token->type == END)
	{
		(*n)->right = init_tree_node(token, ms);
		(*n)->right->parent = *n;
	}
	if (!(*n)->value && token->type != END)
		(*n)->value = ft_strdup(token->value);
	if (token->type != END)
		add_cmd_arg(&((*n)->cmd_args), token->value);
}

void	parse_pipe(t_token *token, t_tree_node **n, t_ms_var *ms)
{
	while ((*n)->parent)
		*n = (*n)->parent;
	(*n)->parent = init_tree_node(token, ms);
	(*n)->parent->left = *n;
	if (token->next->type != END)
	{
		(*n)->parent->right = init_tree_node(token->next, ms);
		(*n)->parent->right->parent = (*n)->parent;
		*n = (*n)->parent->right;
	}
	else
		*n = (*n)->parent;
}

void	parse(t_token *token, t_tree_node **n, t_ms_var *ms, t_token *head)
{
	if (!*n)
		*n = init_tree_node(token, ms);
	if (token->type == PIPE)
		parse_pipe(token, n, ms);
	else if (token->type >= REDIRECT_IN && token->type <= APPEND)
	{
		parse_redir(token, &(*n)->redir);
		token = token->next;
	}
	else if (token)
		parse_word(token, n, ms);
	if (token->next)
		parse(token->next, n, ms, head);
	else
		free_tokens(head);
}
