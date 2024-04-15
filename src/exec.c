/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:37:57 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/15 15:23:26 by ledelbec         ###   ########.fr       */
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

int	code_for_errno(void)
{
	if (errno == EACCES)
		return (126);
	else if (errno == ENOENT)
		return (127);
	return (-1);
}

int	create_child(t_minishell *msh, t_node *node, int in, int out)
{
	int		pid;
	char	*cmd;

	cmd = NULL;
	pid = fork();
	if (pid == -1)
		return (msh_errno(""), -1);
	if (pid == 0)
	{
		if (node->cmd.argc > 0)
		{
			cmd = ft_create_path(msh, node->cmd.argv[0]);
			if (!cmd)
				return (msh_error_cmd(node->cmd.argv[0]), close_fd_child(msh),
					exit(code_for_errno()), 0);
		}
		overall_dup(node, in, out);
		close_fd_child(msh);
		if (cmd)
			ft_exec_cmd(cmd, node->cmd.argv, msh->env);
		exit(0);
	}
	ft_vector_add(&msh->child_pids, &pid);
	close_fd_parent(in, out);
	return (0);
}

int	exec_cmd(t_minishell *msh, t_node *node, int parent_in, int parent_out)
{
	if (node->type == TY_CMD)
	{
		if (is_builtin(node))
			return (exec_builtin(msh, node, parent_in, parent_out));
		else
			return (create_child(msh, node, parent_in, parent_out));
	}
	return (handle_if_not_cmd(msh, node, parent_in, parent_out));
}

int	wait_for_children(t_minishell *msh)
{
	size_t	i;
	int		status;

	status = 0;
	while (wait(&status) > 0)
	{
		i = 0;
		if (g_signum != -1)
		{
			while (i < ft_vector_size(msh->child_pids))
			{
				kill(msh->child_pids[i], g_signum);
				i++;
			}
			g_signum = -1;
		}
	}
	return (WEXITSTATUS(status));
}
