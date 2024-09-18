/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingmohamedalsherif <kingmohamedalsherif@s +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:19:16 by chon              #+#    #+#             */
/*   Updated: 2024/09/17 15:41:09 by kingmohamedalshe ###   ########.fr       */
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

typedef struct s_lst
{
	char			*var_n_val;
	char			*var;
	char			*val;
	int				ascii_order;
	struct s_lst	*bwd;
	struct s_lst	*fwd;
}	t_lst;

typedef struct s_paths
{
	char 	**split_filepaths;
	char 	*filepath_0;
	char 	*filepath;
	char	**env;
}	t_paths;

typedef enum
{
	WORD,
	CMD,
	ARG,
	PIPE,
	AND,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND,
	OR,
	CD,
	END,
	OPEN_Q
} token_type;

typedef struct s_var
{
	char	*start;
	char	*str;
	int		len;
	char	quote;
	char	*blockers;
	char	*std_blockers;
	char	*sgl_quote_block;
	char	*dbl_quote_block;
	char	*std_block_ex_quotes;
	char	*usd_blockers;
	bool	convert;
}	t_var;

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
	char		**cmd_args_arr;
	struct s_token *next;
}	t_token;

// (In_file redirection)
typedef struct s_redir
{
	char 			 *filename;
	int 			 is_heredoc;
	char 			 *heredoc_delim;
	int 			 regular_infile;
	int 			 is_append;
	int 			 regular_outfile;
	struct s_redir   *next;
} t_redir;

typedef struct s_ms_var
{
	t_lst	*env;
	char	**env_arr;
	t_lst	*exp;
}	t_ms_var;

typedef struct s_tree_node
{
	t_token				*token;
	t_paths				*p;
	t_args				*args;
	t_redir 			*redir;
	t_redir 			*in_fd_node;
	t_redir 			*out_fd_node;
	// int 				empty_fd;
	int					**pipefd;
	struct s_tree_node	*parent;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	int					is_first_node;
	int					is_last_node;
	bool				is_read;
	char				*value;
	t_lst				*ms_env;
	t_lst				*ms_export;
} t_tree_node;

// typedef struct s_exec
// {
// 	char ***cmd_args;
// 	int *pid;
// 	int infile;
// 	int outfile;
// 	int orig_v_c_s;
// 	int pipe_ct;
// 	char **env;
// } t_exec;

// PARSING FUNCS
t_args 		*create_arg_node(char *value);
void 		tokenize(char *input, t_token **tokens, t_lst *env);
void 		print_tokens(t_token *tokens);
void 		add_arg(t_args **args, t_args *new_arg);
void		free_tokens(t_token *tokens);
void    	add_token(t_token **tokens, char *str, token_type type);
char    	*get_env(char *input, t_lst *env);
// void    	mv_ptr_incr_len(char **input, int *len);
bool		valid_quote_pairs(char *input);
char		sngl_or_dbl(char *input);
char		*get_substr(char **input, char *blocker);

int 		parsing(t_token **tokens, t_tree_node **ast, t_ms_var *ms);
int 		init_cmd(t_token *token, t_tree_node **ast);
void		create_fds(t_tree_node **ast, t_ms_var *ms);
void		init_tree_node(t_tree_node **new_node, t_token *token);
void 		add_end_node(t_tree_node **ast);
void 		handle_redir(t_redir **redir, char *value, token_type type);

void 		ft_error(int error, char *str, t_tree_node *p, int exit_switch);
void		free_char_arr(char **twoD, char ***threeD);
void		free_int_array(int **twoD, int cmd_ct);
void		free_all(t_tree_node *n);
void		check_filepaths(t_tree_node *head);
int			is_empty(char *av);
t_tree_node	*start_node(t_tree_node *n);
void		traverse_tree(t_tree_node **n);
void		init_exec(t_tree_node *n, int pipe_ct);
void		reset_read_flag(t_tree_node **n);
void 		rl_replace_line(const char *text, int clear_undo);
void 		rl_clear_history (void);
void		cd(t_tree_node *n);
void 		close_fds(t_tree_node *n, int pipe_ct);
int			count_lst_nodes(t_lst *head);
void		dup_env_exp(t_ms_var **ms, char **env);
int			is_number(char *str);
int			has_valid_chars(char *str);
void		insert_node(t_tree_node *n, char *str);
t_lst		*create_new_node(char *str, int ascii_order);
void		update_order(t_lst *head, t_lst *node);
char		*remove_quotes(char *str);
char		*export_str(char *str);
void		del_node(t_lst *n, int rank);
void		free_lst_node(t_lst *node);
int			parse(int ac, char **av, char **env);

#endif