# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/22 14:41:53 by ledelbec          #+#    #+#              #
#    Updated: 2024/03/22 16:36:05 by ledelbec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS=\
	src/main.c \
	src/prompt.c

OBJS=$(SRCS:.c=.o)
DEPS=$(OBJS:.o=.d)

CC=clang
CFLAGS=-Ilibft -O2 -fno-builtin -Wall -Wextra #-Werror
NAME=minishell

all: $(NAME)

$(NAME): $(OBJS) libft/libft.a
	$(CC) -o $(NAME) $(OBJS) libft/libft.a -lreadline

libft/libft.a:
	make -C libft

clean:
	make -C libft clean
	rm -f $(OBJS) $(DEPS)

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re libft

