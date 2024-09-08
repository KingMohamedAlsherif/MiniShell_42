/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingmohamedalsherif <kingmohamedalsherif@s +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:19:16 by chon              #+#    #+#             */
/*   Updated: 2024/09/08 17:43:16 by kingmohamedalshe ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# 	define MINISHELL_H

#define MALLOC_ERROR 1
#define SYNTAX_ERROR 2
#define UNCLOUSED_Q 3

# include "./libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <errno.h>
# include <ctype.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>

// note: tokenize cmd and args as one token; 
	// create program breakers for syntax errors such as 
	// i)	redirection immediately followed by |
	// ii)	unclosed quotes
	// iii)	() without && or ||

typedef struct s_paths
{
	char 	*all_filepaths;
	char 	**split_filepaths;
	char 	*filepath_0;
	char 	*filepath;
	char	**env;
} t_paths;

typedef enum
{
	WORD,
	CMD,
	ARG,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND,
	DOUBLE_Q,
	SINGLE_Q,
	OR,
	AND,
	SPACE
} token_type;


typedef	struct s_args
{
	char	*arg;
	struct s_args *next;
} 		t_args;


typedef struct s_token
{
	token_type	type;
	char 		*value;
	char 		*exec_cmd_path;
	t_args		*cmd_args;
	struct s_token *next;
} t_token;

// (In_file redirection)
typedef struct s_redir
{
	char 			 *filename;
	int 			 is_heredoc;
	char 			 *heredoc_delim;   // Delimiter for heredoc
	int 			 regular_infile;	
	int 			 is_append;
	int 			 regular_outfile;	
	struct s_redir   *next;
} t_redir;

typedef struct s_tree_node
{
	t_token				*token;
	t_paths				*p;
	token_type			type;
	t_args				*args;
	t_redir 			*redir;
	int					*in_fds;
	int					*out_fds;
	int					in_fd;
	int					out_fd;
	char				*infile;
	char				*outfile;
	int					is_here_doc;
	char				*delimiter;
	
	// int 				empty_fd;
	int					*pipefd;
	struct s_tree_node	*parent;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	int					is_first_node;
	int					is_last_node;
	bool				is_read;
	char		*value;
} t_tree_node;

typedef struct s_exec
{
	char ***cmd_args;
	int *pid;
	int infile;
	int outfile;
	int orig_v_c_s;
	int pipe_ct;
	char **env;
} t_exec;

// PARSING FUNCS
t_args 		*create_arg_node(char *value);
void 		add_arg(t_args **args, t_args *new_arg);
int 		parsing(t_token **tokens, t_tree_node **AST);
int 		parse_word(t_token *token, t_tree_node **AST);
int 		parse_pip(t_token *token, t_tree_node **AST);
t_token 	*tokenize(char **input, t_token **tokens);
void 		print_tokens(t_token *tokens);
int 		init_cmd(t_token *token, t_tree_node **ast);



void pipex(t_exec *p, char *infile);
void ft_error(int error, char *str, t_tree_node *p, int exit_switch);
void free_char_arr(char **twoD, char ***threeD);
void free_int_array(int **twoD, int cmd_ct);
void free_all(t_tree_node *n);
void setup_p_cp_arr(t_exec *p);
void close_fds(t_tree_node *n);
void check_filepaths(t_tree_node *head);
int is_empty(char *av);
t_tree_node *start_node(t_tree_node *n);
void traverse_tree(t_tree_node **n);
void init_exec(t_tree_node *n);
void reset_read_flag(t_tree_node **n);
void rl_replace_line(const char *text, int clear_undo);
void rl_clear_history(void);

#endif