/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env-variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:52:22 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/01 20:51:09 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include "test.h"

void	setup_node(t_node *node)
{
	const char *argv1[] = { "cd", "/home/ledelbec", NULL };

	node->type = TY_CMD;
	node->cmd.argv = ft_vector_from_array((void *) argv1, 3);
	node->cmd.argc = 2;
	node->cmd.outfile = NULL;
	node->cmd.append = false;
	node->cmd.infile = NULL;
}

int	main()
{
	char		*line;
	t_minishell	msh;
	t_node		node;
	char		*s;

	line = "cd $HOME";
	msh.env = ft_vector(sizeof(char *), 0);
	s = "HOME=/home/ledelbec";
	ft_vector_add(&msh.env, &s);
	s = NULL;
	ft_vector_add(&msh.env, &s);
	setup_node(&node);
	assert_node(line, parse_line(&msh, line), &node);
	return (0);
}
