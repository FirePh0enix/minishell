/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:37:57 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/13 16:24:00 by vopekdas         ###   ########.fr       */
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

bool	is_builtin(t_node *node)
{
	if (node->cmd.argc == 0)
		return (false);
	if (strcmp(node->cmd.argv[0], "cd") == 0)
		return (true);
	else if (strcmp(node->cmd.argv[0], "pwd") == 0)
		return  (true);
	else if (strcmp(node->cmd.argv[0], "echo") == 0)
		return  (true);
	else if (strcmp(node->cmd.argv[0], "exit") == 0)
		return (true);
	else if (strcmp(node->cmd.argv[0], "unset") == 0)
		return (true);
	else if (strcmp(node->cmd.argv[0], "env") == 0)
		return (true);
	else if (strcmp(node->cmd.argv[0], "export") == 0)
		return (true);
	return (false);
}

int	exec_builtin(t_minishell *msh, t_node *node, int in, int out)
{
	int	ac;

	ac = node->cmd.argc;
	if (strcmp(node->cmd.argv[0], "cd") == 0)
		return (builtin_cd(msh, ac, node->cmd.argv, in, out, node));
	else if (strcmp(node->cmd.argv[0], "pwd") == 0)
		return (builtin_pwd(ac, node->cmd.argv, out, node));
	else if (strcmp(node->cmd.argv[0], "echo") == 0)
		return (builtin_echo(ac, node->cmd.argv, out, node));
	else if (strcmp(node->cmd.argv[0], "exit") == 0)
		return (builtin_exit(ac, node->cmd.argv, in, out, node));
	else if (strcmp(node->cmd.argv[0], "unset") == 0)
		return (builtin_unset(msh, ac, node->cmd.argv, node));
	else if (strcmp(node->cmd.argv[0], "env") == 0)
		return (builtin_env(msh, in, out, node));
	else if (strcmp(node->cmd.argv[0], "export") == 0)
		return (builtin_export(msh, ac, node->cmd.argv, in, out, node));
	return (0);
}

int	code_for_errno(void)
{
	if (errno == EACCES)
		return (126);
	else if (errno == ENOENT)
		return (127);
	return (-1);
}

int    exec_cmd(t_minishell *msh, t_node *node, int parent_in, int parent_out)
{
	int		pid;
	char	*cmd;
	int		status;

	status = 0;
	cmd = NULL;
	if (node->type == TY_CMD)
	{
		if (is_builtin(node))
			return (exec_builtin(msh, node, parent_in, parent_out));
		if (node->cmd.argc > 0)
		{
			cmd = ft_create_path(msh, node->cmd.argv[0]);
			if (!cmd)
				return (msh_error_cmd(node->cmd.argv[0]), code_for_errno());
		}
		pid = fork();
		if (pid == -1)
			return (-1);
		if (pid == 0)
		{
			overall_dup(node, parent_in, parent_out);
			for (size_t i = 0; i < ft_vector_size(msh->open_fds); i++)
				close(msh->open_fds[i]);
			ft_vector_free(msh->open_fds);
			if (cmd)
				ft_exec_cmd(cmd, node->cmd.argv, msh->env);
			else
				exit(0);
		}
		else
			close_fd_parent(parent_in, parent_out);
	}
	else if (node->type == TY_PIPE)
		return (handle_pipe(msh, node, parent_in, parent_out));
	else if (node->type == TY_OR)
		return (handle_or(msh, node, parent_in, parent_out));
	else if (node->type == TY_AND)
		return (handle_and(msh, node, parent_in, parent_out));
	return (status);
}
