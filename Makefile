# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/22 14:41:53 by ledelbec          #+#    #+#              #
#    Updated: 2024/04/14 12:13:47 by ledelbec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS=\
	src/prompt.c \
	src/signal.c \
	src/parser.c \
	src/parser/tokenizer.c \
	src/parser/expr.c \
	src/parser/free_node.c \
	src/parser/expand_tokens.c \
	src/wildcard.c \
	src/env.c \
	src/exec_utils.c \
	src/exec.c \
	src/history.c \
	src/error.c \
	src/builtins/cd.c \
	src/builtins/echo.c \
	src/builtins/env.c \
	src/builtins/exit.c \
	src/builtins/export.c \
	src/builtins/pwd.c \
	src/builtins/unset.c \
	src/close_fd.c\
	src/dup.c\
	src/handle_type.c\
	src/builtins_utils.c\

MAIN_SRCS=src/main.c $(SRCS)

OBJS=$(MAIN_SRCS:.c=.o)
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

