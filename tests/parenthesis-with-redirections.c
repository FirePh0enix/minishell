/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis-with-redirections.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 20:10:07 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/01 20:49:19 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include "test.h"

void	setup_node(t_node *node)
{
	const char	*argv1[] = { "echo", "Hello world", NULL };
	const char	*argv2[] = { "grep", "all", NULL };
	t_node		*l;
	t_node		*r;

	node->type = TY_PIPE;
	l = ft_calloc(1, sizeof(t_node));
	l->type = TY_CMD;
	l->cmd.argv = ft_vector_from_array((char **) argv1, 3);
	l->cmd.argc = 2;
	r = ft_calloc(1, sizeof(t_node));
	r->cmd.argv = ft_vector_from_array((char **) argv2, 3);
	r->cmd.argc = 2;
	r->cmd.outfile = "test.txt";
	node->pipe.left = l;
	node->pipe.right = r;
}

int	main()
{
	char		*line;
	t_minishell	msh;
	t_node		node;

	line = "(echo \"Hello world\" | grep all) > test.txt";
	setup_node(&node);
	assert_node(line, parse_line(&msh, line), &node);
	return (0);
}
