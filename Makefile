# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/22 14:41:53 by ledelbec          #+#    #+#              #
#    Updated: 2024/04/20 19:15:22 by ledelbec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS=\
	src/prompt.c \
	src/prompt_utils.c \
	src/signal.c \
	src/signal_utils.c \
	src/history.c \
	src/error.c \
	src/env.c \
	src/parser.c \
	src/parser/tokenizer.c \
	src/parser/tokenizer_utils.c \
	src/parser/expr.c \
	src/parser/expr_utils.c \
	src/parser/expr_utils2.c \
	src/parser/heredoc.c \
	src/parser/handle_redirects_parent.c \
	src/parser/parse_cmd.c \
	src/parser/free_node.c \
	src/parser/expand_tokens.c \
	src/parser/expand_tokens_utils.c \
	src/parser/expand_env.c \
	src/parser/expand_env_utils.c \
	src/parser/expand_wildcards.c \
	src/parser/wildcard.c \
	src/parser/wildcard_utils.c \
	src/parser/clean.c \
	src/exec/exec_utils.c \
	src/exec/exec.c \
	src/exec/close_fd.c \
	src/exec/dup.c \
	src/exec/handle_type.c \
	src/exec/builtins_utils.c \
	src/builtins/cd.c \
	src/builtins/echo.c \
	src/builtins/env.c \
	src/builtins/exit.c \
	src/builtins/export.c \
	src/builtins/export_utils.c \
	src/builtins/pwd.c \
	src/builtins/unset.c \
	src/builtins/utils.c

MAIN_SRCS=src/main.c $(SRCS)

OBJS=$(MAIN_SRCS:.c=.o)
DEPS=$(OBJS:.o=.d)

CC=clang
CFLAGS=-Ilibft -Isrc -MMD -O2 -fno-builtin -g -Wall -Wextra #-Werror
NAME=minishell

all: $(NAME)

$(NAME): $(OBJS) libft/libft.a
	$(CC) -o $(NAME) $(OBJS) libft/libft.a -lreadline

build-test: CFLAGS+=-DTEST=1
build-test: $(NAME)

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

