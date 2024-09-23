/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:19:16 by chon              #+#    #+#             */
/*   Updated: 2024/09/23 14:43:04 by chon             ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>

typedef struct s_lst
{
	char			*var_n_val;
	char			*var;
	char			*val;
	int				ascii_order;
	struct s_lst	*bwd;
	struct s_lst	*fwd;
}	t_lst;

typedef struct s_ms_var
{
	t_lst	*env;
	char	**env_arr;
	t_lst	*exp;
}	t_ms_var;

typedef enum
{
	WORD,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND,
	PIPE,
	AND,
	OR,
	CD,
	END
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

typedef struct s_paths
{
	char 	**split_filepaths;
	char 	*filepath_0;
	char 	*filepath;
}	t_paths;

typedef	struct s_args
{
	char	*arg;
	struct s_args *next;
}	t_args;

typedef struct s_token
{
	token_type	type;
	char 		*value;
	struct s_token *next;
}	t_token;

typedef struct s_redir
{
	char 			 *filename;
	char 			 *heredoc_delim;
	int 			 in_fd;
	int 			 is_append;
	int 			 out_fd;
	struct s_redir   *fwd;
	struct s_redir   *bwd;
} t_redir;

typedef struct s_tree_node
{
	token_type			type;
	char				*value;
	t_args				*cmd_args;
	char				**cmd_args_arr;
	char 				*exec_cmd_path;
	t_paths				*p;
	t_redir 			*redir;
	int					pipe_ct;
	int					**pipefd;
	struct s_tree_node	*parent;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	bool				is_read;
	t_ms_var			*ms;
} t_tree_node;

void 		rl_replace_line(const char *text, int clear_undo);
void 		rl_clear_history (void);
void		dup_env_exp(t_ms_var **ms, char **env);
char		*export_str(char *str);

// PARSING FUNCS
void 		tokenize(char *input, t_token **tokens, t_lst *env);
void 		print_tokens(t_token *tokens);
void		free_tokens(t_token *tokens);
void    	add_token(t_token **tokens, char *str, token_type type);
char    	*get_env(char *input, t_lst *env);
bool		valid_quote_pairs(char *input);
char		*get_substr(char **input, char *blocker);
void 		add_cmd_arg(t_args **args_lst, char *value);
bool 		syntax_errors(t_token *tokens_list);

void 		parse(t_token *token, t_tree_node **ast, t_ms_var *ms);
void		pipes_n_exec_path(t_tree_node *head, t_ms_var *ms, int *pipe_ct);
t_tree_node	*init_tree_node(t_token *token, t_ms_var *ms);
void		update_node(t_redir *new_redir, char *value, token_type type);

void 		ft_error(int error, char *str, t_tree_node *p, int exit_switch);
void		check_filepaths(t_tree_node *head);
t_tree_node	*start_node(t_tree_node *n);
void		traverse_tree(t_tree_node **n);
void		init_exec(t_tree_node *n, int pipe_ct);
void		cd(t_tree_node *n);
void 		close_fds(t_tree_node *n, int pipe_ct);
int			is_number(char *str);
int			count_lst_nodes(t_lst *head);
int			has_valid_chars(char *str);
void		insert_node(t_tree_node *n, char *str);
t_lst		*create_new_node(char *str, int ascii_order);
void		update_order(t_lst *head, t_lst *node);
char		*remove_quotes(char *str);
void		del_node(t_lst *n, int rank);
void		last_redir_fd(t_redir *redir, char type, int *fd);

bool		is_builtin(t_tree_node *n, char *cmd);
void		export(t_tree_node *n);

void		free_char_arr(char **two_d, char ***three_d);
void		free_lst(t_lst *lst);
void		free_tree(t_tree_node *n);
void		traverse_tree_to_free(t_tree_node **n, int is_read_flag);

#endif