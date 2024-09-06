/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:19:16 by chon              #+#    #+#             */
/*   Updated: 2024/09/06 13:20:40 by chon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
	char			*str;
	int				ascii_order;
	struct s_lst	*bwd;
	struct s_lst	*fwd;
}	t_lst;

typedef struct s_paths
{
	char 	*all_filepaths;
	char 	**split_filepaths;
	char 	*filepath_0;
	char 	*filepath;
	char	**env;
}	t_paths;

typedef enum
{
	CMD,
	ARG,
	PIPE,
	AND,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	CD,
	END
}	token_type;

typedef struct s_token
{
	token_type	type;
	char 		*str;
	char 		*exec_cmd_path;
	char		**cmd_args;
	struct s_token *next;
}	t_token;

typedef struct s_tree_node
{
	t_token				*token;
	t_paths				*p;
	int					in_fd;
	int					out_fd;
	char				*infile;
	char				*outfile;
	int					is_here_doc;
	char				*delimiter;
	// int 				empty_fd;
	int					**pipefd;
	struct s_tree_node	*parent;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	int					is_first_node;
	int					is_last_node;
	bool				is_read;
	t_lst				*ms_env;
	t_lst				*ms_export;
}	t_tree_node;

void		ft_error(int error, char *str, t_tree_node *p, int exit_switch);
void		free_char_arr(char **twoD, char ***threeD);
void		free_int_array(int **twoD, int cmd_ct);
void		free_all(t_tree_node *n);
void		check_filepaths(t_tree_node *head);
int			is_empty(char *av);
t_tree_node	*start_node(t_tree_node *n);
void		traverse_tree(t_tree_node **n);
void		init_exec(t_tree_node *n, int pipe_ct, int cmd_ct);
void		reset_read_flag(t_tree_node **n);
void 		rl_replace_line(const char *text, int clear_undo);
void 		rl_clear_history (void);
void		cd(t_tree_node *n);
void 		close_fds(t_tree_node *n, int pipe_ct);
int			count_lst_nodes(t_lst *head);
void		create_env_export(t_lst **env_head, t_lst **export_head, char **env);
int			is_number(char *str);
int			has_valid_chars(char *str);
void		insert_node(t_tree_node *n, char *str);
t_lst		*new_node(char *str, int ascii_order);
void		update_order(t_lst *head, t_lst *node);
char		*remove_quotes(char *str);
char		*export_str(char *str);
void		del_node(t_tree_node *n, int rank);

#endif