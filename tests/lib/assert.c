/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assert.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 22:14:52 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/01 20:44:09 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../test.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

static bool	not_all_null(char *a, char *b)
{
	return ((a != NULL && b == NULL) || (a == NULL && b != NULL));
}

static bool	node_equals(t_node *a, t_node *b)
{
	int	i;

	if (a->type != b->type)
		return (false);
	if (a->type == TY_CMD && b->type == TY_CMD)
	{
		if (a->cmd.argc != b->cmd.argc)
			return (false);
		i = 0;
		while (i < a->cmd.argc)
		{
			if (strcmp(a->cmd.argv[i], b->cmd.argv[i]))
				return (false);
			i++;
		}
		if (not_all_null(a->cmd.outfile, b->cmd.outfile)
			|| (a->cmd.outfile != NULL && b->cmd.outfile != NULL && strcmp(a->cmd.outfile, b->cmd.outfile)))
			return (false);
	}
	else if (a->type == TY_PIPE && b->type == TY_PIPE)
		return (a->pipe.left && b->pipe.left
			&& node_equals(a->pipe.left, b->pipe.left)
			&& a->pipe.right && b->pipe.right
			&& node_equals(a->pipe.right, b->pipe.right));
	return (true);
}

void	assert_node(char *line, t_node *got, t_node *expected)
{
	if (node_equals(got, expected))
		return ;
	printf("Assertion failed when parsing `%s`\n", line);
	printf("Got ");
	dump_line(got);
	printf("\nwhen expecting ");
	dump_line(expected);
	exit(1);
}
