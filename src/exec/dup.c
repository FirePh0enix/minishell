/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:17:13 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/23 16:58:45 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dup_infile(t_node *node)
{
	int		file;

	file = open(node->cmd.infile, O_RDONLY);
	if (file == -1)
		return (msh_errno(node->cmd.infile), exit(1), 1);
	if (node->cmd.argc > 0 && dup2(file, STDIN_FILENO) == -1)
		return (msh_errno(""), exit(1), 1);
	close(file);
	return (0);
}

int	dup_outfile(t_node *node)
{
	int	flags;
	int	file;

	flags = O_WRONLY | O_CREAT;
	if (node->cmd.append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	file = open(node->cmd.outfile, flags, 0666);
	if (file == -1)
		return (msh_errno(node->cmd.outfile), exit(1), 1);
	if (dup2(file, STDOUT_FILENO) == -1)
		return (msh_errno(""), exit(1), 1);
	close(file);
	return (0);
}

int	dup_in(int in)
{
	if (in != -1 && dup2(in, STDIN_FILENO) == -1)
		return (msh_errno(""), exit(1), 1);
	return (0);
}

int	dup_out(int out)
{
	if (out != -1 && dup2(out, STDOUT_FILENO) == -1)
		return (msh_errno(""), exit(1), 1);
	return (0);
}

void	overall_dup(t_node *node, int in, int out)
{
	if (in != -1)
		dup_in(in);
	if (node->cmd.infile)
		dup_infile(node);
	if (out != -1)
		dup_out(out);
	if (node->cmd.outfile)
		dup_outfile(node);
	return ;
}
