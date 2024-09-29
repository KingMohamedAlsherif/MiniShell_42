/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malsheri <malsheri@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 02:57:51 by malsheri          #+#    #+#             */
/*   Updated: 2024/09/29 02:58:52 by malsheri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(t_token *token)
{
	t_token	*tmp_token;

	while (token)
	{
		tmp_token = token;
		token = token->next;
		free(tmp_token->value);
		free(tmp_token);
	}
}

void	update_node(t_redir *new_redir, char *value, token_type type)
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

void	parse_word(t_token *token, t_tree_node **ast, t_ms_var *ms)
{
	if (token->type == END)
	{
		(*ast)->right = init_tree_node(token, ms);
		(*ast)->right->parent = *ast;
	}
	if (!(*ast)->value && token->type != END)
		(*ast)->value = ft_strdup(token->value);
	if (token->type != END)
		add_cmd_arg(&((*ast)->cmd_args), token->value);
}

void	parse(t_token *token, t_tree_node **ast, t_ms_var *ms, t_token *head)
{
	if (!*ast)
		*ast = init_tree_node(token, ms);
	if (token->type == PIPE || token->type == AND || token->type == OR)
	{
		while ((*ast)->parent)
			*ast = (*ast)->parent;
		(*ast)->parent = init_tree_node(token, ms);
		(*ast)->parent->left = *ast;
		(*ast)->parent->right = init_tree_node(token->next, ms);
		(*ast)->parent->right->parent = (*ast)->parent;
		*ast = (*ast)->parent->right;
	}
	else if (token->type >= REDIRECT_IN && token->type <= APPEND)
	{
		parse_redir(token, &(*ast)->redir);
		token = token->next;
	}
	else if (token)
		parse_word(token, ast, ms);
	if (token->next)
		parse(token->next, ast, ms, head);
	else
		free_tokens(head);
}
