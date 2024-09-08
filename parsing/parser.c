#include "../minishell.h"

int         parsing(t_token     **tokens, t_tree_node   **ast)
{
    t_token *token;
    int     ret;

    ret = 0;
    token = *tokens;
    if (token && token->type == WORD)
        ret = parse_word(token, ast);
    else if (token && token->type == PIPE)
        ret = parse_pip(token, ast);
    // else if (token && token->type == REDIRECT_IN || token->type == REDIRECT_OUT 
    // || token->type == REDIRECT_OUT || token->type == REDIRECT_OUT)
    //     ret = parse_redir(token, ast);
    if (token != NULL)
        return (parsing(&token->next, ast));
    return (ret);
}

// int parse_word(t_token *token, t_tree_node **ast)
// {
//     t_tree_node *new_tree;
//     t_args *new_arg;

//     // Case 1: If the current AST node is a command, add the token as an argument
//     if ((*ast) && (*ast)->type == CMD)
//     {
//         new_arg = create_arg_node(token->value);
//         if (!new_arg)
//             return (MALLOC_ERROR);
//         add_arg(&((*ast)->token->cmd_args), new_arg);
//         return (0);
//     }

//     // Case 2: Create a new command node
//     new_tree = malloc(sizeof(t_tree_node));
//     if (!new_tree)
//         return (MALLOC_ERROR);

//     // Initialize the new command node
//     new_tree->parent = NULL;
//     new_tree->type = CMD;
//     new_tree->token = token;
//     new_tree->left = NULL;
//     new_tree->right = NULL;

//     // Case 3: If the previous node was a pipe, attach the command as the right child
//     if ((*ast) && (*ast)->type == PIPE)
//     {
//         (*ast)->right = new_tree; // Attach the command to the right of the pipe
//         new_tree->parent = *ast;  // Set the pipe as the parent of the command
//         *ast = new_tree;          // Move the AST pointer to the new command node
//     }
//     // Case 4: If there is no pipe, this becomes a new root
//     else
//     {
//         *ast = new_tree;
//     }

//     // Create the argument for the command
//     new_arg = create_arg_node(new_tree->token->value);
//     if (!new_arg)
//         return (MALLOC_ERROR);
//     add_arg(&(new_tree->token->cmd_args), new_arg);

//     return (0);
// }

int     parse_word(t_token  *token, t_tree_node **ast)
{
    t_tree_node *new_tree;
    t_args *new_arg;

    if ((*ast) && (*ast)->type == CMD)
    {
        new_arg = create_arg_node(token->value);
        if (!new_arg)
            return (MALLOC_ERROR);
        add_arg(&((*ast)->token->cmd_args), new_arg);
        return (0);
    }
    new_tree = malloc(sizeof(t_tree_node));
    if (!new_tree)
        return (MALLOC_ERROR);
    new_tree->type = CMD;
    new_tree->token = token;
    new_tree->left = NULL;
    new_tree->right = NULL;
    new_arg = create_arg_node(new_tree->token->value);
    if (!new_arg)
        return (MALLOC_ERROR);
    add_arg(&(new_tree->token->cmd_args), new_arg);
    if((*ast) && (*ast)->type == PIPE)
    {
        if ((*ast)->right)
         {
             new_arg = create_arg_node(token->value);
             if (!new_arg)
                 return (MALLOC_ERROR);
             add_arg(&((*ast)->right->token->cmd_args), new_arg);
             return (0);
         }
        (*ast)->right = new_tree; // cmd to right
        new_tree->parent = *ast;  // pipe as the parent
    }
    else
        *ast = new_tree;
    return (0);
}



int     parse_pip(t_token  *token, t_tree_node     **ast)
{
    t_tree_node *new_tree;

    if (!(*ast))
    {
        printf("SYNTAX_ERROR\n");
        return (SYNTAX_ERROR);
    }
    new_tree = malloc(sizeof(t_tree_node));
    if (!new_tree)
    {
        printf("MALLOC_ERROR\n");
        return (MALLOC_ERROR);
    }
    new_tree->token = token;
    new_tree->type = PIPE;
    new_tree->parent = NULL;
    new_tree->left = *ast;
    new_tree->right = NULL;
    if ((*ast)->type == CMD)
        (*ast)->parent = new_tree;
    *ast = new_tree;
    return (0);
}

int         parse_Redir(t_token     *token, t_tree_node     **ast)
{
    if (!(*ast) || (*ast)->type != CMD)
        return (SYNTAX_ERROR);

    t_tree_node *new_tree;
    new_tree = malloc(sizeof(t_tree_node));
    if (!new_tree)
    {
        printf("MALLOC_ERROR\n");
        return (MALLOC_ERROR);
    }
    new_tree->token = token;
    new_tree->type = PIPE;
    new_tree->parent = NULL;
    new_tree->left = *ast;
    new_tree->right = NULL;
    if ((*ast)->type == CMD)
        (*ast)->parent = new_tree;
    *ast = new_tree;
}

// int parse_redirection(t_token *token, t_tree_node **ast)
// {
//     // Ensure the current AST node is a command (or create a new command node)
//     if (!(*ast) || (*ast)->type != CMD)
//     {
//         printf("SYNTAX_ERROR: Redirection without a command\n");
//         return SYNTAX_ERROR;
//     }

//     t_tree_node *cmd_node = *ast; // Command node where we attach redirection

//     // Identify the type of redirection and handle accordingly
//     if (token->type == REDIR_IN)
//     {
//         // Handle input redirection
//         if (cmd_node->in_count < 10)
//         {
//             cmd_node->in_files[cmd_node->in_count] = open(token->value, O_RDONLY);
//             if (cmd_node->in_files[cmd_node->in_count] == -1)
//                 return FILE_OPEN_ERROR;
//             cmd_node->in_count++;
//         }
//     }
//     else if (token->type == REDIR_OUT)
//     {
//         // Handle output redirection (overwrite)
//         if (cmd_node->out_count < 10)
//         {
//             cmd_node->out_files[cmd_node->out_count] = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             if (cmd_node->out_files[cmd_node->out_count] == -1)
//                 return FILE_OPEN_ERROR;
//             cmd_node->out_count++;
//         }
//     }
//     else if (token->type == REDIR_APPEND)
//     {
//         // Handle output redirection (append)
//         if (cmd_node->out_count < 10)
//         {
//             cmd_node->out_files[cmd_node->out_count] = open(token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
//             if (cmd_node->out_files[cmd_node->out_count] == -1)
//                 return FILE_OPEN_ERROR;
//             cmd_node->out_count++;
//         }
//     }

//     return 0;
// }
