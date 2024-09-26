#include "minishell.h"

t_tree_node	*start_node(t_tree_node *n)
{
	while (n->parent)
		n = n->parent;
	while (n->left)
		n = n->left;
	return (n);
}

void	traverse_tree(t_tree_node **n)
{
	bool	read_flag;
	bool	unread_flag;

	if ((start_node(*n)) == *n)
		read_flag = ((*n)->is_read + 1) % 2;
	else
		read_flag = start_node(*n)->is_read;
	unread_flag = (read_flag + 1) % 2;
	(*n)->is_read = read_flag;
	if ((*n)->type == END)
		return ;
	if ((*n)->parent && ((!(*n)->left && !(*n)->right)
		|| ((*n)->left && (*n)->left->is_read == read_flag
			&& (*n)->right && (*n)->right->is_read == read_flag)))
		*n = (*n)->parent;
	else if ((*n)->left && (*n)->left->is_read == unread_flag)
		*n = (*n)->left;
	else
		*n = (*n)->right;
}

void	traverse_tree_to_free(t_tree_node **n, int is_read_flag)
{
	bool	unread_flag;

	unread_flag = (is_read_flag + 1) % 2;
	(*n)->is_read = is_read_flag;
	if ((*n)->type == END)
		return ;
	if ((*n)->parent && ((!(*n)->left && !(*n)->right)
		|| ((*n)->left && (*n)->left->is_read == is_read_flag
			&& (*n)->right && (*n)->right->is_read == is_read_flag)))
		*n = (*n)->parent;
	else if ((*n)->left && (*n)->left->is_read == unread_flag)
		*n = (*n)->left;
	else
		*n = (*n)->right;
}
