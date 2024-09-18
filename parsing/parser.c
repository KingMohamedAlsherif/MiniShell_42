#include "../minishell.h"

void	parse_redir(t_token **token, t_tree_node **ast)
{
    if (!token || !*token || !ast || !*ast || !(*token)->next) // ensure that next token is word
        exit (1);
    handle_redir(&((*ast)->redir), (*token)->next->value, (*token)->type);
    *token = (*token)->next;
    // printf("Current token value: %s\n", (*token)->value);
}

void	parse_pipe(t_token  *token, t_tree_node     **ast)
{
    t_tree_node *new_t_node;

	while ((*ast)->parent)
		*ast = (*ast)->parent;
	new_t_node = init_tree_node(token);
    new_t_node->left = *ast;
	(*ast)->parent = new_t_node;
    *ast = new_t_node;
}

void	parse_word(t_token *token, t_tree_node **ast)
{
    t_tree_node	*new_t_node;

	if (!*ast || (*ast)->token->type == PIPE || token->type == END)
	{
		new_t_node = init_tree_node(token);
		add_cmd_arg(&new_t_node->token->cmd_args, token->value);
		if (*ast)
		{
			if ((*ast)->token->type == PIPE && token->type == END)
				(*ast) = (*ast)->right;
			(*ast)->right = new_t_node;
			new_t_node->parent = *ast;
		}
		*ast = new_t_node;
	}
	else
		add_cmd_arg(&((*ast)->token->cmd_args), token->value);
}

void	parse(t_token **tokens, t_tree_node **ast)
{
    t_token		*token;

    token = *tokens;
	printf("token: %s\n", token->value);
    if (token && token->type == PIPE)
		parse_pipe(token, ast);
    else if (token && (token->type == REDIRECT_IN || token->type == REDIRECT_OUT 
		|| token->type == HEREDOC || token->type == APPEND))
        parse_redir(tokens, ast);
    else if (token)
		parse_word(token, ast);
	if (token->next)
		parse(&token->next, ast);
}
