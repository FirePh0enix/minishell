# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/22 14:41:53 by ledelbec          #+#    #+#              #
#    Updated: 2024/03/26 19:39:34 by ledelbec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS=\
	src/main.c \
	src/prompt.c \
	src/signal.c \
	src/parser.c \
	src/wildcard.c \
	src/env.c \
	src/exec.c \
	src/builtins/cd.c\
	src/builtins/echo.c\
	src/builtins/env.c\
	src/builtins/exit.c\
	src/builtins/export.c\
	src/builtins/pwd.c\
	src/builtins/unset.c\

OBJS=$(SRCS:.c=.o)
DEPS=$(OBJS:.o=.d)

CC=clang
CFLAGS=-Ilibft -Isrc -MMD -O2 -fno-builtin -g -Wall -Wextra #-Werror
NAME=minishell

all: $(NAME)

$(NAME): $(OBJS) libft/libft.a
	$(CC) -o $(NAME) $(OBJS) libft/libft.a -lreadline

libft/libft.a:
	make -C libft

-include $(DEPS)
include tests.mk

clean:
	make -C libft clean
	rm -f $(OBJS) $(DEPS)

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re libft

