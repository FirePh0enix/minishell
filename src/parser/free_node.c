/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:28:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/25 16:56:04 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stdlib.h>

void	free_node(t_node *node)
{
	if (!node)
		return ;
	if (node->type == TY_CMD)
	{
		if (node->cmd.argv)
			ft_vector_deep_free(node->cmd.argv);
		if (node->cmd.env)
			ft_vector_deep_free(node->cmd.env);
		if (node->cmd.outfile)
			free(node->cmd.outfile);
		if (node->cmd.infile)
			free(node->cmd.infile);
	}
	else if (node->type == TY_PARENT)
	{
		free_node(node->pa.node);
	}
	else
	{
		free_node(node->pipe.left);
		free_node(node->pipe.right);
	}
	free(node);
}

static	t_node	*get_origin(t_node *node)
{
	t_node	*origin;

	origin = NULL;
	while (node->parent)
	{
		origin = node->parent;
		node = node->parent;
	}
	if (origin == NULL)
		return (node);
	return (origin);
}

void	free_node_tree(t_node *node)
{
	t_node	*origin;

	origin = get_origin(node);
	free_node(origin);
}

void	free_node_in_child(t_node *node)
{
	if (!node)
		return ;
	if (node->type == TY_CMD)
	{
		if (node->cmd.env)
			ft_vector_deep_free(node->cmd.env);
		if (node->cmd.outfile)
			free(node->cmd.outfile);
		if (node->cmd.infile)
			free(node->cmd.infile);
	}
	else
	{
		free_node(node->pipe.left);
		free_node(node->pipe.right);
	}
	free(node);
}
