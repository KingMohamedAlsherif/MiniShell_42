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

// void	create_ms_env(char ***ms_env, char **env)
// {
// 	char	**ms_env_ptr;
// 	int		str_ct;

// 	str_ct = 0;
// 	while (env[str_ct])
// 		str_ct++;
// 	ms_env_ptr = malloc(sizeof(char *) * (str_ct + 1));
// 	if (!ms_env_ptr)
// 		return ;
// 	str_ct = -1;
// 	while (env[++str_ct])
// 		ms_env_ptr[str_ct] = ft_strdup(env[str_ct]);
// 	ms_env_ptr[str_ct] = NULL;
// 	*ms_env = ms_env_ptr;
// }

void	create_ms_export(t_export **head, t_env *ms_env)
{
	t_export	*head_ptr;
	int			node_ct;
	int			i;
	t_env		env_head;

	head_ptr = malloc(sizeof(t_export));
	if (!head_ptr)
		return ;
	*head = head_ptr;
	node_ct = ft_lstsize(ms_env);
	i = -1;
	while (++i < node_ct)
	{
		while (ms_env->ascii_order != i)
			ms_env = ms_env->next;
		head_ptr->str = ft_strjoin("declare -x ", ms_env->str);
		ms_env = env_head;
		head_ptr->next = malloc(sizeof(t_export));
		if (!head_ptr->next)
			return ;
		head_ptr = head_ptr->next;
	}
}

void	update_order(t_env *head, t_env *node)
{
	int	rank;

	rank = 0;
	while (head)
	{
		if (ft_strncmp(head->str, node->str, ft_strlen(node->str) + 1))

		head = head->next;
	}
}

void	create_ms_env(t_env **head, char **env)
{
	t_env	*head_ptr;
	int		i;

	head_ptr = malloc(sizeof(t_env));
	if (!head_ptr)
		return ;
	*head = head_ptr;
	i = -1;
	while (env[++i])
	{
		head_ptr->str = ft_strdup(env[i]);
		head_ptr->ascii_order = 0;
		if (env[i + 1])
		{
			head_ptr->next = malloc(sizeof(t_env));
			if (!head_ptr->next)
				return ;
		}
		else
			head_ptr->next = NULL;
		head_ptr = head_ptr->next;
	}
	head_ptr = (*head)->next;
	while (head_ptr)
	{
		update_order(*head, head_ptr);
		head_ptr = head_ptr->next;
	}
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	char		**split_input;
	// char		**ms_env;
	t_env		*ms_env;
	t_export	*ms_export;

	(void)ac;
	(void)av;
	if (init_signals())
		return (1);
	create_ms_env(&ms_env, env);
	create_ms_export(&ms_export, ms_env);
	printf("%s\n", ms_env->next->str);
	while (1)
	{
		input = readline("Minishell $ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		split_input = ft_split(input, ' ');
		if (!ft_strncmp(split_input[0], "clear", 6) && !split_input[1])
			rl_clear_history();
		add_history(input);
		// free_char_arr(split_input, NULL);
		free(input);
	}
	return (0);
}
