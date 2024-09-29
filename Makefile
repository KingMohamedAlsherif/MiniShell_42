# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chon <chon@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/10 11:20:21 by chon              #+#    #+#              #
#    Updated: 2024/09/29 22:44:57 by chon             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CFLAGS = -Wall -Werror -Wextra -g3
LIBFT_DIR = ./libft
LIBS = ./libft/libft.a -L/Users/chon/.local/opt/readline/lib \
-I//Users/chon/.local/opt/readline/include -lreadline
OBJS = $(SRCS:.c=.o)
SRCS = main.c signals.c env_export.c utils.c mv_tree.c free.c \
tokenize/tokenize.c tokenize/utils.c \
./execute/pipes_path.c ./execute/exec.c ./execute/utils.c ./execute/fcntl.c\
./builtins/builtins.c ./builtins/export.c ./builtins/utils.c \
./parsing/parser.c ./parsing/utils.c ./builtins/cd.c\

$(NAME): $(OBJS)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

all: $(NAME)

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJS)
	rm -f $(OBJS_B)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY = all clean fclean re