// #include "minishell.h"

// int parse_word(t_token *token, t_tree_node **AST)
// {
//     t_tree_node *new_tree;
//     t_args *new_arg;

//     // Check for null pointers to avoid dereferencing issues
//     if (!token || !AST || !(*AST))
//         return (NULL_POINTER_ERROR);

//     // Create a new argument node for the token value
//     new_arg = create_arg_node(token->value);
//     if (!new_arg)
//         return (MALLOC_ERROR);

//     // If current AST node is a WORD, add the argument to its command arguments
//     if ((*AST)->type == WORD)
//     {
//         add_arg(&((*AST)->token->cmd_args), new_arg);
//         return (0);
//     }

//     // Allocate memory for a new tree node
//     new_tree = malloc(sizeof(t_tree_node));
//     if (!new_tree)
//         return (MALLOC_ERROR);

//     // Initialize the new tree node
//     new_tree->parent = NULL;
//     new_tree->type = CMD;
//     new_tree->token = token;
//     new_tree->left = NULL;
//     new_tree->right = NULL;

//     // Add the argument to the new tree node's command arguments
//     add_arg(&(new_tree->token->cmd_args), new_arg);

//     // If the parent node is a PIPE, link the new node to the right of it
//     if ((*AST)->type == PIPE)
//     {
//         new_tree->parent = (*AST);
//         (*AST)->right = new_tree;
//     }

//     return (0);
// }

// int parse_pipe(t_token *token, t_tree_node **AST)
// {
//     t_tree_node *new_tree;

//     // Check for null pointers to avoid dereferencing issues
//     if (!token || !AST || !(*AST))
//         return (NULL_POINTER_ERROR);

//     // Allocate memory for a new tree node (pipe node)
//     new_tree = malloc(sizeof(t_tree_node));
//     if (!new_tree)
//         return (MALLOC_ERROR);

//     // Initialize the new tree node as a PIPE node
//     new_tree->parent = NULL;
//     new_tree->type = PIPE;
//     new_tree->token = token;
//     new_tree->left = *AST;  // Connect the current AST to the left side of the pipe
//     new_tree->right = NULL;

//     // Set the parent of the current AST node to the new pipe node
//     (*AST)->parent = new_tree;

//     // Update the AST to point to the new pipe node
//     *AST = new_tree;

//     return (0);
// }
