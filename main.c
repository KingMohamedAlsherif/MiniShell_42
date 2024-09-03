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
	char		**split_input;
	t_env		*ms_env;
	t_export	*ms_export;

	(void)ac;
	(void)av;
	if (init_signals())
		return (1);
	create_env_export(&ms_env, &ms_export, env);
	while (1)
	{
		input = readline("Minishell $ ");
		split_input = ft_split(input, ' ');
		if (!input || !ft_strncmp(split_input[0], "exit", 5))
		{
			if (split_input[1] && !is_number(split_input[1]))
				printf("exit\nbash: exit: %s: numeric argument required\n"
					, split_input[1]);
			else
				printf("exit\n");
			break ;
		}
		if (!ft_strncmp(split_input[0], "clear", 6) && !split_input[1])
			rl_clear_history();
		add_history(input);
		// free_char_arr(split_input, NULL);
		free(input);
	}
	return (0);
}
