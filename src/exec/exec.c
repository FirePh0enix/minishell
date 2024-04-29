/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:37:57 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/29 12:02:33 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_child(t_minishell *msh, t_node *node, int in, int out)
{
	char	*cmd;

	cmd = NULL;
	if (node->cmd.argc > 0)
	{
		cmd = ft_create_path(msh, node->cmd.argv[0]);
		if (!cmd)
		{
			msh_error_cmd(node->cmd.argv[0]);
			free_when_no_cmd(msh, node);
			errno = ENOENT;
			return (exit(code_for_errno()), 0);
		}
		else if (cmd == (void *) 1 || cmd == (void *) 2)
		{
			msh_errno(node->cmd.argv[0]);
			return (free_when_no_cmd(msh, node), exit(code_for_cmd(cmd)), 0);
		}
	}
	add_env(msh, node);
	overall_dup(node, in, out);
	close_fd_child(msh);
	if (cmd)
		exec_cmd_when_cmd_ok(msh, cmd, node);
	free_at_end(msh, node);
}

int	create_child(t_minishell *msh, t_node *node, int in, int out)
{
	int		pid;

	if (node->cmd.argc == 0)
		add_env(msh, node);
	pid = fork();
	if (pid == -1)
		return (msh_errno(""), -1);
	if (pid == 0)
		exec_child(msh, node, in, out);
	ft_vector_add(&msh->child_pids, &pid);
	close_fd_parent(in, out);
	if (!node->parent || node->parent->type != TY_PIPE)
		return (wait_for_children(msh));
	return (0);
}

static int	check_all_redirects(t_node *node)
{
	size_t	i;
	int		fd;
	t_red	red;

	i = 0;
	while (i < ft_vector_size(node->cmd.all_reds))
	{
		red = node->cmd.all_reds[i];
		if (red.type == RED_IN)
			fd = open(red.filename, O_RDONLY);
		else if (red.type == RED_OUT && red.append)
			fd = open(red.filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
		else if (red.type == RED_OUT && !red.append)
			fd = open(red.filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd == -1)
		{
			msh_errno(red.filename);
			return (-1);
		}
		close(fd);
		i++;
	}
	return (0);
}

int	exec_cmd(t_minishell *msh, t_node *node, int parent_in, int parent_out)
{
	if (node->type == TY_CMD)
	{
		if (check_all_redirects(node) == -1)
			return (1);
		if (is_builtin(node))
			return (exec_builtin(msh, node, parent_in, parent_out));
		else
			return (create_child(msh, node, parent_in, parent_out));
	}
	return (handle_if_not_cmd(msh, node, parent_in, parent_out));
}

int	wait_for_children(t_minishell *msh)
{
	int		i;
	int		status;
	int		override_status;

	status = 0;
	override_status = -1;
	while (wait(&status) > 0)
	{
		if (g_signum != -1)
		{
			i = 0;
			while (++i < (int)ft_vector_size(msh->child_pids))
				kill(msh->child_pids[i], g_signum);
			if (g_signum == SIGINT)
				override_status = 130;
			else if (g_signum == SIGQUIT)
				override_status = 131;
			g_signum = -1;
		}
	}
	if (override_status != -1)
		return (override_status);
	return (WEXITSTATUS(status));
}
