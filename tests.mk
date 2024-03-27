# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    tests.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/26 19:37:47 by ledelbec          #+#    #+#              #
#    Updated: 2024/03/27 11:51:53 by ledelbec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

TEST_LIB=\
	tests/lib/assert.c

TEST_SRCS=\
	$(TEST_LIB) \
	src/prompt.c \
	src/signal.c \
	src/parser.c \
	src/wildcard.c \
	src/env.c \
	src/exec.c \
	src/builtins/cd.c \
	src/builtins/echo.c \
	src/builtins/env.c \
	src/builtins/exit.c \
	src/builtins/export.c \
	src/builtins/pwd.c \
	src/builtins/unset.c

TESTS=\
	simple-command-parsing \
	command-parsing-with-pipe \
	env-variables \

test:
	@$(foreach TEST, $(TESTS), bash tests/test.sh $(TEST) "$(TEST_SRCS)";)
	@rm -f minishell.test

test-clean:
	rm -f minishell.test
