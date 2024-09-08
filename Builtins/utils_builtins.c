/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:36:20 by chon              #+#    #+#             */
/*   Updated: 2024/09/04 16:30:09 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	insert_node(t_tree_node *n, char *str)
{
	t_lst	*env_node;
	t_lst	*export_node;
	t_lst	*tmp_node;
	int		rank;

	env_node = n->ms_env;
	while (env_node->next)
		env_node = env_node->next;
	env_node->next = new_node(ft_strdup(str), 0);
	update_order(n->ms_env, env_node);
	rank = env_node->ascii_order;
	export_node = n->ms_export;
	while (export_node->ascii_order < rank - 1)
		export_node = export_node->next;
	tmp_node = export_node->next;
	export_node->next = new_node(export_str(str), rank);
	export_node->next->next = tmp_node;
	export_node = tmp_node;
	while (export_node)
	{
		export_node->ascii_order++;
		export_node = export_node->next;
	}
	free(str);
}

char	*remove_quotes(char *str)
{
	char	*str_ex_quotes;
	int		quote_count;
	int		i;
	int		j;

	quote_count = 0;
	i = -1;
	while (str[++i])
		if (str[i] == '\"' || str[i] == '\'')
			quote_count++;
	str_ex_quotes = malloc(ft_strlen(str) - quote_count + 1);
	if (!str_ex_quotes)
		exit (1);
	i = 0;
	j = -1;
	while (str[i])
	{
		while (str[i] == '\"' || str[i] == '\'')
			i++;
		while (str[i] != '\"' && str[i] != '\'')
			str_ex_quotes[++j] = str[i++];
	}
	str_ex_quotes[i] = '\0';
	return (str_ex_quotes);
}

int	has_valid_chars(char *str)
{
	while (*str)
	{
		if (*str < 'A' || *str > 'z')
			return (0);
		str++;
	}
	return (1);
}
