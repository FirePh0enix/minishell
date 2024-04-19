/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:28:21 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/19 12:30:26 by ledelbec         ###   ########.fr       */
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
		ft_vector_deep_free(node->cmd.argv);
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
