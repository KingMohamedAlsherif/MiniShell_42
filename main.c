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
	{
		// rl_replace_line("", 0);
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
		signal_received = 1;
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

int	main(void)
{
	char	*input;

	if (init_signals())
		return (1);
	while (1)
	{
		input = readline("Minishell $ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (signal_received)
		{
			add_history(input);
			free(input);
			signal_received = 0;
			continue ;
		}
		if (!ft_strncmp(input, "clear", 6))
			clear_history();
		add_history(input);
		free(input);
	}
	return (0);
}
