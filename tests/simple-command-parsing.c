/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple-command-parsing.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 22:00:52 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/01 20:50:19 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "test.h"
#include <stdio.h>

int	main()
{
	char		*line;
	const char	*argv[] = { "ls", "test.sh" };
	t_minishell	msh;
	t_node		node;

	line = "ls test.sh";
	node.type = TY_CMD;
	node.cmd.argc = 2;
	node.cmd.argv = ft_vector(sizeof(char *), 0);
	node.cmd.outfile = NULL;
	node.cmd.append = false;
	node.cmd.infile = NULL;
	msh.env = ft_vector(sizeof(char *), 0);
	for (int i = 0; i < 2; i++)
		ft_vector_add(&node.cmd.argv, &argv[i]);
	assert_node(line, parse_line(&msh, line), &node);
	return (0);
}
