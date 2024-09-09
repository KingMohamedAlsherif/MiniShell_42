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

// volatile	sig_atomic_t signal_received;

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

// 	sa.sa_handler = signal_handler;
// 	sa.sa_flags = 0;
//     sigemptyset(&sa.sa_mask);
// 	if (sigaction(SIGINT, &sa, NULL) < 0)
// 	{
// 		perror("sigaction");
// 		return (1);
// 	}
// 	return (0);
// }

// void print_node_data(int type)
// {
// 	if (type)
// 		printf("%d\n", type); // Modify this according to your data type
// }

// void print_tree(t_tree_node *root)
// {
// 	if (root == NULL)
// 		return;

// 	// Print current node's data
// 	print_node_data(root->type);

// 	// Traverse left subtree
// 	print_tree(root->left);

// 	// Traverse right subtree
// 	print_tree(root->right);
// }

// void print_tree(t_tree_node *node, int level)
// {
// 	if (!node)
// 		return;

// 	// Indent based on tree level
// 	for (int i = 0; i < level; i++)
// 	{
// 		printf("  ");
// 	}

// 	// Print the current node data
// 	printf("Node (type: %d, value: %s)\n", node->type, node->token->value);

// 	// Recursively print left and right children
// 	print_tree(node->left, level + 1);
// 	print_tree(node->right, level + 1);
// }

void print_args(t_args *args)
{
	t_args *current = args;

	if (!current)
	{
		printf("No arguments to display.\n");
		return;
	}

	while (current)
	{
		if (current->arg != NULL) // Check if the argument is not NULL
		{
			printf("Argument: %s\n", current->arg);
		}
		else
		{
			printf("Argument is NULL\n"); // If it's NULL, print this message
		}
		current = current->next;
	}
}

void	print_tree(t_tree_node	*ast)
{
	if (ast)
	{
		printf("AST ==> (( %s )) and Type => %d\n ", ast->token->value, ast->type);
		if (ast->args)
			print_args(&(ast->args));
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
}

int	main(void)
{
	char	*input;
	// char	**split_input;
	t_tree_node *ast;
	t_token *tokens;

	ast = NULL;
	tokens = NULL;
	// if (init_signals())
	// 	return (1);
	while (1)
	{
		input = readline("Minishell $ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		tokenize(&input, &tokens);
		parsing(&tokens, &ast);
		print_tree(ast);
		// if (parsing(&tokens, &ast) == 1 || parsing(&tokens, &ast) == 2)
		// {
		// 	// print_tree(ast);
		// 	printf("A7a\n");
		// }
				// printf("Return Vlue => 0\n");
		// print_tokens(tokens);
			// printf("It's Working\n");
		// print_tree(*ast);
		// split_input = ft_split(input, ' ');
		// // printf("%s %d\n", split_input[0], ft_strncmp(split_input[0], "clear", 5));
		// if (!ft_strncmp(split_input[0], "clear", 6) && !split_input[1])
		// {
		// 	rl_clear_history();
		// 	printf("hits\n");
		// }
		// add_history(input);
		// free_char_arr(split_input, NULL);
		// free(input);
	}
	return (0);
}
