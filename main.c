/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:08:29 by chon              #+#    #+#             */
/*   Updated: 2024/09/30 02:08:43 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

void	init_ms(char *input, t_ms_var *ms)
{
	t_token		*tokens;
	t_tree_node	*n;
	int			pipe_ct;
	char		*str;

	str = NULL;
	tokens = NULL;
	add_history(input);
	tokenize(input, &tokens, ms->env, str);
	free(input);
	if (tokens && !syntax_errors(tokens))
	{
		n = NULL;
		parse(tokens, &n, ms, tokens);
		pipes_n_exec_path(start_node(n), ms, &pipe_ct);
		init_exec(start_node(n), pipe_ct);
		traverse_and_check_errors(start_node(n));
		n->is_read = start_node(n)->is_read;
		free_tree(start_node(n));
	}
}

bool	exit_ms(char *input, t_ms_var *ms)
{
	char	**split_input;

	split_input = ft_split(input, ' ');
	if (!input || (split_input[0] && !ft_strncmp(split_input[0], "exit", 5)))
	{
		if (input && split_input[1] && !is_number(split_input[1]))
			printf("exit\nbash: exit: %s: numeric argument required\n",
				split_input[1]);
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

bool	is_empty(char *input)
{
	while (*input)
	{
		if (!ft_strchr(" \n\t\f\v\r\n", *input))
			return (0);
		input++;
	}
	return (1);
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
		while (1)
		{
			if (init_signals())
				return (1);
			input = readline("Minishell $ ");
			if (exit_ms(input, ms))
				break ;
			if ((ft_strlen(input) > 5 && !ft_strncmp(input, "clear ", 6))
				|| (ft_strlen(input) < 6 && !ft_strncmp(input, "clear", 5)))
				printf("\033[2J\033[H");
			else if (ms->env && !is_empty(input))
				init_ms(input, ms);
		}
	}
	return (0);
}
