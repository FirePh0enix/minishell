/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:37:57 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/22 16:49:38 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void	add_env(t_minishell *msh, t_node *node)
{
	size_t	i;
	char	*s;
	char	*env;
	char	*name;
	char	*value;

	i = 0;
	while (i < ft_vector_size(node->cmd.env))
	{
		env = node->cmd.env[i];
		s = ft_strchr(env, '=');
		name = ft_strndup(env, s - env);
		value = ft_strdup(s + 1);
		if (ft_strcmp(name, "_"))
			setourenv(msh, name, value);
		free(name);
		free(value);
		i++;
	}
}

static int	exec_child(t_minishell *msh, t_node *node, int in, int out)
{
	char	*cmd;
	char	**av;

	cmd = NULL;
	if (node->cmd.argc > 0)
	{
		cmd = ft_create_path(msh, node->cmd.argv[0]);
		if (!cmd)
		{
			free_env(msh);
			free_history(msh);
			ft_vector_free(msh->child_pids);
			close_fd_child(msh);
			rl_clear_history();
			msh_error_cmd(node->cmd.argv[0]);
			free_node(node);
			return (exit(code_for_errno()), 0);
		}
	}
	add_env(msh, node);
	overall_dup(node, in, out);
	close_fd_child(msh);
	if (cmd)
	{
		av = node->cmd.argv;
		free_node_in_child(node);
		ft_exec_cmd(msh, cmd, av, msh->env);
	}
	free_env(msh);
	free_history(msh);
	ft_vector_free(msh->child_pids);
	rl_clear_history();
	free_node(node);
	exit(0);
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
