/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:42:49 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/19 14:43:26 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_outfile(t_node *node)
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
