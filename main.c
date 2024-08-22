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

void	signal_handler(int g_sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (g_sig == SIGINT)
	{
		// write(STDOUT_FILENO, "\n", 1);
		// write(STDOUT_FILENO, "\nMinishell $ ", 13);
		// printf("\n");
		signal_received = 1;
	}
}

int	main(void)
{
	char	*input;
	struct sigaction sa;

	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror("sigaction");
		return (1);
	}
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
			printf("\b\bexit\n");
			break ;
		}
		add_history(input);
		free(input);
	}
	return (0);
}