# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    tests.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/26 19:37:47 by ledelbec          #+#    #+#              #
#    Updated: 2024/03/27 14:03:51 by vopekdas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

TEST_LIB=\
	tests/lib/assert.c

TEST_SRCS:=\
	$(TEST_LIB) \
	$(SRCS)

TESTS=\
	simple-command-parsing \
	command-parsing-with-pipe \
	env-variables \

test: libft/libft.a
	@$(foreach TEST, $(TESTS), bash tests/test.sh $(TEST) "$(TEST_SRCS)";)
	@rm -f minishell.test

test-clean:
	rm -f minishell.test
