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

// void	signal_handler(int signum)
// {
// 	if (signum == SIGINT)
// 	{
// 		// printf("\033[12C  ");
// 		printf("\n");
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 		rl_redisplay();
// 	}
// }

// int	init_signals(void)
// {
// 	struct sigaction sa;

// 	// sa.sa_handler = signal_handler;
// 	// sa.sa_flags = 0;
//     // sigemptyset(&sa.sa_mask);
// 	if (sigaction(SIGINT, &sa, NULL) < 0)
// 	{
// 		perror("sigaction");
// 		return (1);
// 	}
// 	return (0);
// }

void print_args(t_args *tokens)
{
	while (tokens)
	{
		printf("ARG => %s\n", tokens->arg);
		tokens = tokens->next;
	}
}
void print_redir(t_redir *tokens)
{
	while (tokens)
	{
		printf("Redir => %s\n", tokens->filename);
		tokens = tokens->next;
	}
}

void	print_tree(t_tree_node	*ast)
{
	if (ast && ast->token)
	{
		printf("AST ==> (( %s )) and Type => %d\n", ast->token->value, ast->type);
	}
	if (ast && ast->right)
	{
		printf("RIGHT =>> ");
		print_tree(ast->right);
	}
	if (ast && ast->left)
	{
		printf("LIFT =>>");
		print_tree(ast->left);
	}
	if (ast->token->cmd_args != NULL)
		print_args(ast->token->cmd_args);
	if (ast->redir != NULL)
		print_redir(ast->redir);
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	char		**split_input;
	t_tree_node	*ast;
	t_token		*tokens;
	t_lst		*ms_env;
	t_lst		*ms_export;

	(void)ac;
	(void)av;
	// ast = NULL;
	// if (init_signals())
	// 	return (1);
	create_env_export(&ms_env, &ms_export, env);
	while (1)
	{
		input = readline("Minishell $ ");
		split_input = ft_split(input, ' ');
		// if (!ft_strncmp(split_input[0], "clear", 6) && !split_input[1])
		// 	rl_clear_history();
		if (!input || !env || (split_input[0] && !ft_strncmp(split_input[0], "exit", 5)))
		{
			if (input && split_input[1] && !is_number(split_input[1]))
				printf("exit\nbash: exit: %s: numeric argument required\n"
					, split_input[1]);
			else
				printf("exit\n");
			free_char_arr(split_input, NULL);
			break ;
		}
		else if (env)
		{
			add_history(input);
			tokenize(input, &tokens, ms_env);
			print_tokens(tokens);
			parsing(&tokens, &ast);
			print_tree(ast);
			free_char_arr(split_input, NULL);
			free(input);
			free_tokens(tokens);
		}
	}
	return (0);
}
