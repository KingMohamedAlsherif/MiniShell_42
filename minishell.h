/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chon <chon@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:19:16 by chon              #+#    #+#             */
/*   Updated: 2024/08/25 15:15:09 by chon             ###   ########.fr       */
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

typedef enum
{
	CMD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
} token_type;

typedef struct s_token
{
	token_type	type;
	char 		*str;
	char 		*exec_cmd_path;
	char		**cmd_args;
	struct s_token *next;
} t_token;

typedef struct s_paths
{
	char 		*all_filepaths;
	char 		**split_filepaths;
	char 		*filepath_0;
	char 		*filepath;
} t_paths;

typedef struct s_tree_node
{
	t_token		token;
	t_paths		p;
	int			in_fd;
	int			out_fd;
	char		*outfile;
	int			is_here_doc;
	char		*delimiter;
	int 		empty_fd;
	int			**pipefd;
<<<<<<< HEAD
	bool		is_last_node;
	t_tree_node	*parent;
	t_tree_node	*left;
	t_tree_node	*right;
=======
	struct s_tree_node	*parent;
	struct s_tree_node	*left;
	struct s_tree_node	*right;
	int			is_last_node;
	bool		is_read;
>>>>>>> 9a837cc87971b70f5d4eba414c430dd2067fcb42
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

void		pipex(t_exec *p, char *infile);
void		ft_error(int error, char *str, t_tree_node *p, int exit_switch);
void		free_char_arr(char **twoD, char ***threeD);
void		free_int_array(int **twoD, int cmd_ct);
void		free_all(t_exec *p);
void		setup_p_cp_arr(t_exec *p);
void		close_fds(t_exec *p);
void		check_filepaths(t_tree_node *head);
int			is_empty(char *av);
t_tree_node	*start_node(t_tree_node *n);
void		traverse_tree(t_tree_node **n);

#endif