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

void	signal_handler(int signum)
{
	if (signum == SIGINT)
		signal_received = 1;
}

void	init_signals()
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
}

int	main(void)
{
	char	*input;

	init_signals();
	while (1)
	{
		input = readline("Minishell $ ");
		if (!input)
		{
			if (signal_received)
			{
				signal_received = 0;
				write(STDOUT_FILENO, input, ft_strlen(input));
				free(input);
				printf("\b\b  \n");
				continue ;
			}
			// printf("\b\bexit\n");
			printf("exit\n");
			break ;
		}
		// if (!ft_strncmp(input, "clear", 6))
		// 	rl_clear_history();
		add_history(input);
		free(input);
	}
	return (0);
}
