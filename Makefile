# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chon <chon@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/10 11:20:21 by chon              #+#    #+#              #
#    Updated: 2024/08/21 13:42:29 by chon             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CFLAGS = -Wall -Werror -Wextra -g3
LIBFT_DIR = ./libft
LIBS = ./libft/libft.a -lreadline
OBJS = $(SRCS:.c=.o)
SRCS = main.c \
# ./execute/exec_main.c ./execute/pipex.c ./execute/utils.c ./execute/free.c
# OBJS_B = $(SRCS_B:.c=.o)
# SRCS_B = 

$(NAME): $(OBJS)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

all: $(NAME)

# bonus: $(OBJS_B)
# 	make -C $(LIBFT_DIR)
# 	$(CC) $(CFLAGS) $(OBJS_B) $(LIBS) -o $(NAME)

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJS)
	rm -f $(OBJS_B)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY = all clean fclean re