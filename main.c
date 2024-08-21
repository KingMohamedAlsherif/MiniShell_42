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

volatile sig_atomic_t g_sig;

void	handle_signal(int g_sig)
{
	if (g_sig == -1)
		exit(0);
}

int	main(void)
{
	char	*input;
	struct sigaction sa;

    sa.sa_handler = handle_signal;
	sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
	while (1)
	{
		input = readline("Minishell $ ");
		if (!input)
			break ;
		// if (!ft_strncmp(input, "clear", 6))
		// 	rl_clear_history();
		else
		{
			if (sigaction(SIGINT, &sa, NULL) < 0)
				perror("sigaction");
			add_history(input);
		}
		free(input);
	}
	return (0);
}