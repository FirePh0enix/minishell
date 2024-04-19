/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:13:31 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/19 12:16:54 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*create_node(t_minishell *msh, t_node *node)
{
	t_node	*node2;
	int		i;
	char	*s;

	(void) msh;
	node2 = ft_calloc(1, sizeof(t_node));
	node2->type = TY_CMD;
	node2->cmd.argv = ft_vector(sizeof(char *), node->cmd.argc - 1);
	node2->cmd.outfile = node->cmd.outfile;
	node2->cmd.infile = node->cmd.infile;
	node2->cmd.append = node->cmd.append;
	node2->cmd.env = ft_vector(sizeof(char *), 0);
	s = "_=/usr/bin/env";
	ft_vector_add(&node2->cmd.env, &s);
	i = 1;
	while (i < node->cmd.argc)
	{
		s = node->cmd.argv[i];
		ft_vector_add(&node2->cmd.argv, &s);
		i++;
	}
	s = NULL;
	ft_vector_add(&node2->cmd.argv, &s);
	node2->cmd.argc = ft_vector_size(node2->cmd.argv) - 1;
	return (node2);
}

static	int	create_outfile(t_node *node)
{
	int	flags;
	int	file;

	flags = O_WRONLY | O_CREAT;
	if (node->cmd.append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	file = open(node->cmd.outfile, flags, 0666);
	return (file);
}

int	builtin_env(t_minishell *msh, int parent_in, int parent_out, t_node *node)
{
	int		i;
	int		file;
	int		exit_code;
	t_node	*node2;

	if (node->cmd.argc >= 2)
	{
		node2 = create_node(msh, node);
		exit_code = exec_cmd(msh, node2, parent_in, parent_out);
		free_node(node2);
		return (exit_code);
	}
	file = STDOUT_FILENO;
	exit_code = 0;
	if (parent_out != -1)
		file = parent_out;
	if (node->cmd.outfile)
		file = create_outfile(node);
	if (node->cmd.argc == 1)
	{
		i = -1;
		while (msh->env[++i])
			ft_putendl_fd(msh->env[i], file);
	}
	if (node->cmd.outfile)
		close(file);
	return (exit_code);
}
