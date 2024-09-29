/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:08:29 by chon              #+#    #+#             */
/*   Updated: 2024/09/29 22:56:47 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t signal = 0;

void print_args(t_args *cmd_args)
{
	printf("args: ");
	while (cmd_args)
	{
		printf("%s ", cmd_args->arg);
		cmd_args = cmd_args->next;
	}
	printf("\n");
}

void print_redir(t_redir *redir, char type)
{
	while (redir)
	{
		if (type == 'i' && (redir->in_fd || redir->heredoc_delim))
			printf("%s ", redir->filename);
		else if (type == 'o' && (redir->out_fd || redir->is_append))
			printf("%s ", redir->filename);
		redir = redir->fwd;
	}
	printf("\n");
}

void print_tree(t_tree_node *n)
{
	while (n->type != END)
	{
		printf("node type: %d value: %s\n", n->type, n->value);
		if (n->type != PIPE)
		{
			print_args(n->cmd_args);
			if (n->redir)
			{
				while (n->redir->bwd)
					n->redir = n->redir->bwd;
			}
			printf("redir in: ");
			print_redir(n->redir, 'i');
			printf("redir out: ");
			print_redir(n->redir, 'o');
		}
		if (n->parent)
			printf("parent: %s\n", n->parent->value);
		else
			printf("parent: none\n");
		if (n->left)
			printf("left: %s\n", n->left->value);
		else
			printf("left: none\n");
		if (n->right)
			printf("right: %s\n", n->right->value);
		else
			printf("right: none\n");
		printf("\n");
		traverse_tree(&n);
	}
	printf("node type: %d value: %s\n", n->type, n->value);
	if (n->type != PIPE)
	{
		print_args(n->cmd_args);
		if (n->redir)
		{
			while (n->redir->bwd)
				n->redir = n->redir->bwd;
		}
		printf("redir in: ");
		print_redir(n->redir, 'i');
		printf("redir out: ");
		print_redir(n->redir, 'o');
	}
	if (n->parent)
		printf("parent: %s\n", n->parent->value);
	else
		printf("parent: none\n");
	if (n->left)
		printf("left: %s\n", n->left->value);
	else
		printf("left: none\n");
	if (n->right)
		printf("right: %s\n", n->right->value);
	else
		printf("right: none\n");
	printf("\n");
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

void	traverse_and_check_errors(t_tree_node *n)
{
	while (n->type != END)
	{
		if ((n->exec_cmd_path && !ft_strncmp(n->exec_cmd_path, "?", 2)
				&& !is_builtin(n->value)) || (!is_builtin(n->value)
				&& n->exec_cmd_path
				&& !strncmp(n->exec_cmd_path, "invalid", 8)))
			printf("%s: command not found\n", n->value);
		else if (n->value && !is_builtin(n->value)
			&& !strncmp(n->exec_cmd_path, "PATH", 5))
			printf("Minishell: %s: No such file or directory\n", n->value);
		traverse_tree(&n);
	}
}

void	init_ms(char *input, t_ms_var *ms)
{
	t_token		*tokens;
	t_tree_node	*n;
	int			pipe_ct;

	add_history(input);
	tokenize(input, &tokens, ms->env);
	// print_tokens(tokens);
	free(input);
	if (tokens && !syntax_errors(tokens))
	{
		n = NULL;
		parse(tokens, &n, ms, tokens);
		// print_tree(start_node(n));
		pipes_n_exec_path(start_node(n), ms, &pipe_ct);
		init_exec(start_node(n), pipe_ct);
		traverse_and_check_errors(start_node(n));
		n->is_read = start_node(n)->is_read;
		// printf("node: %s %d\n",	n->value, n->type);
		// printf("node read? %d\n", n->is_read);
		// printf("parent node read? %d\n", n->parent->is_read);
		free_tree(start_node(n));
	}
}

bool	exit_ms(char *input, t_ms_var *ms)
{
	char	**split_input;

	split_input = ft_split(input, ' ');
	if (!input || (split_input[0] && !ft_strncmp(split_input[0], "exit", 5)))
	{
		if (input && split_input[1] && !is_number(split_input[1]))
			printf("exit\nbash: exit: %s: numeric argument required\n",
				split_input[1]);
		else
			printf("exit\n");
		free_char_arr(split_input, NULL);
		free(input);
		free_lst(ms->env);
		free_lst(ms->exp);
		free_char_arr(ms->env_arr, NULL);
		free(ms);
		return (1);
	}
	free_char_arr(split_input, NULL);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_ms_var	*ms;

	(void)ac;
	(void)av;
	ms = NULL;
	dup_env_exp(&ms, env);
	if (ms)
	{
		if (init_signals())
			return (1);
		while (1)
		{
			input = readline("Minishell $ ");
			if (exit_ms(input, ms))
				break ;
			if ((ft_strlen(input) > 5 && !ft_strncmp(input, "clear ", 6))
				|| (ft_strlen(input) < 6 && !ft_strncmp(input, "clear", 5)))
				printf("\033[2J\033[H");
			else if (ms->env && *input)
				init_ms(input, ms);
		}
	}
	return (0);
}
