# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chon <chon@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/10 11:20:21 by chon              #+#    #+#              #
#    Updated: 2024/08/31 15:27:55 by chon             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address 
# CFLAGS = -Wall -Werror -Wextra
LIBFT_DIR = ./libft
LIBS = ./libft/libft.a -L$(HOME)/.local/opt/readline/lib -I$(HOME)/.local/opt/readline/include -lreadline
# LIBS = ./libft/libft.a -lreadline
OBJS = $(SRCS:.c=.o)
# SRCS = main.c
SRCS = ./parse/parse.c ./execute/exec.c ./execute/utils.c ./execute/free.c ./builtins/builtins.c
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