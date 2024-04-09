# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    tests.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/26 19:37:47 by ledelbec          #+#    #+#              #
#    Updated: 2024/04/09 15:43:10 by ledelbec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

test: libft/libft.a
	@bash tests/test.sh
	@rm -f minishell.test

test-clean:
	rm -f minishell.test
