/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:13:31 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/23 15:19:01 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*dup_if_exist(char *s)
{
	if (!s)
		return (NULL);
	return (ft_strdup(s));
}

static t_node	*create_node(t_minishell *msh, t_node *node)
{
	t_node	*node2;
	int		i;
	char	*s;

	(void) msh;
	node2 = ft_calloc(1, sizeof(t_node));
	node2->type = TY_CMD;
	node2->cmd.argv = ft_vector(sizeof(char *), node->cmd.argc - 1);
	node2->cmd.outfile = dup_if_exist(node->cmd.outfile);
	node2->cmd.infile = dup_if_exist(node->cmd.infile);
	node2->cmd.append = node->cmd.append;
	node2->cmd.env = ft_vector(sizeof(char *), 0);
	s = ft_strdup("_=/usr/bin/env");
	ft_vector_add(&node2->cmd.env, &s);
	i = 1;
	while (i < node->cmd.argc)
	{
		s = ft_strdup(node->cmd.argv[i]);
		ft_vector_add(&node2->cmd.argv, &s);
		i++;
	}
	s = NULL;
	ft_vector_add(&node2->cmd.argv, &s);
	node2->cmd.argc = ft_vector_size(node2->cmd.argv) - 1;
	return (node2);
}

static int	exec_env(t_minishell *msh, t_node *node, int in, int out)
{
	t_node	*node2;
	int		exit_code;

	node2 = create_node(msh, node);
	msh->node_to_free = node;
	exit_code = create_child(msh, node2, in, out);
	free_node(node2);
	return (exit_code);
}

int	builtin_env(t_minishell *msh, int parent_in, int parent_out, t_node *node)
{
	int		i;
	int		file;
	int		exit_code;

	if (node->cmd.argc >= 2)
		return (exec_env(msh, node, parent_in, parent_out));
	exit_code = 0;
	file = STDOUT_FILENO;
	if (parent_out != -1)
		file = parent_out;
	if (node->cmd.outfile)
		file = open_outfile(node);
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
