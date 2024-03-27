/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command-parsing-with-pipe.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:35:55 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/27 11:50:58 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include "test.h"

void	setup_node(t_node *node)
{
	const char *argv1[] = { "cat", "Makefile" };
	const char *argv2[] = { "grep", "all" };

	node->type = TY_PIPE;
	node->pipe.left = malloc(sizeof(t_node));
	node->pipe.left->cmd.argv = ft_vector_from_array((char **) argv1, 2);
	node->pipe.left->cmd.argc = 2;
	node->pipe.right = malloc(sizeof(t_node));
	node->pipe.right->cmd.argv = ft_vector_from_array((char **) argv2, 2);
	node->pipe.right->cmd.argc = 2;
}

int	main()
{
	char		*line;
	t_minishell	msh;
	t_node		node;

	line = "cat    Makefile | grep all";
	msh.env = ft_vector(sizeof(char *), 0);
	setup_node(&node);
	assert_node(line, parse_line(&msh, line), &node);
	return (0);
}
