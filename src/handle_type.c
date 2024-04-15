/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:29:38 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/15 16:06:47 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int	handle_pipe(t_minishell *msh, t_node *node, int in, int out)
{
	int	fd[2];
	int	status;

	if (pipe(fd) == -1)
		return (msh_errno(""), 1);
	ft_vector_add(&msh->open_fds, &fd[0]);
	ft_vector_add(&msh->open_fds, &fd[1]);
	status = exec_cmd(msh, node->pipe.left, in, fd[1]);
	close(fd[1]);
	status = exec_cmd(msh, node->pipe.right, fd[0], out);
	close(fd[0]);
	return (status);
}

int	handle_or(t_minishell *msh, t_node *node, int in, int out)
{
	int	status;

	status = exec_cmd(msh, node->pipe.left, in, out);
	status = wait_for_children(msh);
	if (status == 0)
		return (status);
	status = exec_cmd(msh, node->pipe.right, in, out);
	status = wait_for_children(msh);
	return (status);
}

int	handle_and(t_minishell *msh, t_node *node, int in, int out)
{
	int	status;

	status = exec_cmd(msh, node->pipe.left, in, out);
	status = wait_for_children(msh);
	if (status != 0)
		return (status);
	status = exec_cmd(msh, node->pipe.right, in, out);
	status = wait_for_children(msh);
	return (status);
}

int	handle_parent(t_minishell *msh, t_node *node, int in, int out)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		exit(exec_cmd(msh, node->parent.node, in, out));
	}
	return (wait_for_children(msh));
}

int	handle_if_not_cmd(t_minishell *msh, t_node *node, int in, int out)
{
	if (node->type == TY_PIPE)
		return (handle_pipe(msh, node, in, out));
	else if (node->type == TY_OR)
		return (handle_or(msh, node, in, out));
	else if (node->type == TY_AND)
		return (handle_and(msh, node, in, out));
	else if (node->type == TY_PARENT)
		return (handle_parent(msh, node, in, out));
	return (0);
}
