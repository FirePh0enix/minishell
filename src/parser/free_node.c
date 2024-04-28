/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:28:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/28 12:12:45 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include <stdlib.h>

static void	free_reds(t_red *reds)
{
	size_t	i;

	i = 0;
	while (i < ft_vector_size(reds))
	{
		free(reds[i].filename);
		i++;
	}
	ft_vector_free(reds);
}

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
		if (node->cmd.all_reds)
			free_reds(node->cmd.all_reds);
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
		if (node->cmd.all_reds)
			free_reds(node->cmd.all_reds);
	}
	else
	{
		free_node(node->pipe.left);
		free_node(node->pipe.right);
	}
	free(node);
}
