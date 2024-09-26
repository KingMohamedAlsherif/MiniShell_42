/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:08:29 by chon              #+#    #+#             */
/*   Updated: 2024/08/21 13:08:29 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile	sig_atomic_t signal_received;

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

void	print_tree(t_tree_node	*ast)
{
	while (ast->type != END)
	{
		printf("node type: %d value: %s\n", ast->type, ast->value);
		if (ast->type != PIPE)
		{
			print_args(ast->cmd_args);
			if (ast->redir)
			{
				while(ast->redir->bwd)
					ast->redir = ast->redir->bwd;
			}
			printf("redir in: "); 
			print_redir(ast->redir, 'i');
			printf("redir out: "); 
			print_redir(ast->redir, 'o');
		}
		if (ast->parent)
			printf("parent: %s\n", ast->parent->value);
		else
			printf("parent: none\n");
		if (ast->left)
			printf("left: %s\n", ast->left->value);
		else
			printf("left: none\n");
		if (ast->right)
			printf("right: %s\n", ast->right->value);
		else
			printf("right: none\n");
		printf("\n");
		traverse_tree(&ast);
	}
	printf("node type: %d value: %s\n", ast->type, ast->value);
	if (ast->type != PIPE)
	{
		print_args(ast->cmd_args);
		if (ast->redir)
		{
			while(ast->redir->bwd)
				ast->redir = ast->redir->bwd;
		}
		printf("redir in: "); 
		print_redir(ast->redir, 'i');
		printf("redir out: "); 
		print_redir(ast->redir, 'o');
	}
	if (ast->parent)
		printf("parent: %s\n", ast->parent->value);
	else
		printf("parent: none\n");
	if (ast->left)
		printf("left: %s\n", ast->left->value);
	else
		printf("left: none\n");
	if (ast->right)
		printf("right: %s\n", ast->right->value);
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

void	init_ms(char *input, t_ms_var *ms)
{
	t_token		*tokens;
	t_tree_node	*ast;
	int			pipe_ct;

	add_history(input);
	tokenize(input, &tokens, ms->env);
	free(input);
	// print_tokens(tokens);
	if (tokens && !syntax_errors(tokens))
	{
		ast = NULL;
		parse(tokens, &ast, ms, tokens);
		// print_tree(start_node(ast));
		// printf("%s\n", start_node(ast)->redir->filename);
		pipes_n_exec_path(start_node(ast), ms, &pipe_ct);
		init_exec(start_node(ast), pipe_ct);
		ast = start_node(ast);
		while (ast->type != END)
		{
			if (ast->exec_cmd_path && !ft_strncmp(ast->exec_cmd_path, "?", 2)
				&& !is_builtin(ast->value))
				printf("%s: command not found\n", ast->value);
			traverse_tree(&ast);
		}
		// printf("%d\n", ast->type);
		ast->is_read = start_node(ast)->is_read;
		// printf("%d: read? %d\n", ast->type, ast->is_read);
		// printf("parent of %d: read? %d\n", ast->parent->type, ast->parent->is_read);
		free_tree(start_node(ast));
	}
}

bool	exit_ms(char *input, t_ms_var *ms)
{
	char		**split_input;

	split_input = ft_split(input, ' ');
	if (!input || (split_input[0] && !ft_strncmp(split_input[0], "exit", 5)))
	{
		if (input && split_input[1] && !is_number(split_input[1]))
			printf("exit\nbash: exit: %s: numeric argument required\n"
				, split_input[1]);
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

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	init_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror("sigaction");
		return (1);
	}
	signal(SIGPIPE, SIG_IGN);
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
			else if (ms->env)
				init_ms(input, ms);
		}
	}
	return (0);
}
