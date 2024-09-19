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
	while (ast->token->type != END)
	{
		printf("node type: %d value: %s\n", ast->token->type, ast->token->value);
		if (ast->token->type != PIPE)
		{
			print_args(ast->token->cmd_args);
			printf("redir in: "); 
			print_redir(ast->redir, 'i');
			printf("redir out: "); 
			print_redir(ast->redir, 'o');
		}
		traverse_tree(&ast, 1);
	}
}

void	init_ms(char *input, t_ms_var *ms)
{
	t_token		*tokens;
	t_tree_node	*ast;
	// int			pipe_ct;

	add_history(input);
	tokenize(input, &tokens, ms->env);
	// print_tokens(tokens);
	ast = init_tree_node(token, ms);
	parse(&tokens, &ast, ms);
	print_tree(start_node(ast));
	// pipes_n_exec_path(start_node(ast), ms, &pipe_ct);
	// init_exec(start_node(ast), pipe_ct);
	free(input);
}

bool	exit_ms(char *input)
{
	char		**split_input;

	split_input = ft_split(input, ' ');
	// if (!ft_strncmp(split_input[0], "clear", 6) && !split_input[1])
	// printf("\033[F\033[K");  // Move up and clear
	// 	rl_clear_history();
	if (!input || (split_input[0] && !ft_strncmp(split_input[0], "exit", 5)))
	{
		if (input && split_input[1] && !is_number(split_input[1]))
			printf("exit\nbash: exit: %s: numeric argument required\n"
				, split_input[1]);
		else
			printf("exit\n");
		free_char_arr(split_input, NULL);
		free(input);
		return (1);
	}
	free_char_arr(split_input, NULL);
	return (0);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		// printf("\033[12C  ");
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
			if (exit_ms(input))
				break ;
			if (ms->env)
				init_ms(input, ms);
		}
	}
	return (0);
}
