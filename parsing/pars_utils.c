#include "../minishell.h"

t_tree_node *create_cmd_node(char **argv) {
    t_tree_node *node = malloc(sizeof(t_tree_node));
    node->type = CMD;
    node->argv = argv;
    return node;
}
