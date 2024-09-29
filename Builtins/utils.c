/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 15:36:20 by chon              #+#    #+#             */
/*   Updated: 2024/09/06 13:20:36 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		return (NULL);
	i = 0;
	j = -1;
	while (str[i])
	{
		while (str[i] == '\"' || str[i] == '\'')
			i++;
		while (str[i] && str[i] != '\"' && str[i] != '\'')
			str_ex_quotes[++j] = str[i++];
	}
	str_ex_quotes[i] = '\0';
	return (str_ex_quotes);
}

void	del_node(t_lst *node, int rank)
{
	t_lst	*node_ptr;

	node_ptr = node;
	while (node_ptr->ascii_order != rank)
		node_ptr = node_ptr->fwd;
	if (!node_ptr->bwd)
	{
		node = node_ptr->fwd;
		node->bwd = NULL;
	}
	else if (!node_ptr->fwd)
		node_ptr->bwd->fwd = NULL;
	else
	{
		node_ptr->bwd->fwd = node_ptr->fwd;
		node_ptr->fwd->bwd = node_ptr->bwd;
	}
	free(node_ptr->var_n_val);
	free(node_ptr->var);
	free(node_ptr->val);
	free(node_ptr);
}

int	has_valid_chars(char *str)
{
	if (*str < 'A' || *str > 'z')
		return (0);
	str++;
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9') && (*str < 'A' || *str > 'z')
			&& *str != '=' && *str != ':' && *str != '/' && *str != '.')
			return (0);
		str++;
	}
	return (1);
}
