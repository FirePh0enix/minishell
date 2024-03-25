# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/22 14:41:53 by ledelbec          #+#    #+#              #
#    Updated: 2024/03/25 14:14:22 by vopekdas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS=\
	src/main.c \
	src/prompt.c \
	src/signal.c \
	src/parser.c \
	src/wildcard.c \
	src/env.c \
	src/exec.c

OBJS=$(SRCS:.c=.o)
DEPS=$(OBJS:.o=.d)

CC=clang
CFLAGS=-Ilibft -MMD -O2 -fno-builtin -g -Wall -Wextra #-Werror
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

