/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:28:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/22 16:26:07 by vopekdas         ###   ########.fr       */
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
	else
	{
		free_node(node->pipe.left);
		free_node(node->pipe.right);
	}
	free(node);
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
