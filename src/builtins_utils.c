/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 19:10:48 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/16 12:34:03 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(t_node *node)
{
	if (node->cmd.argc == 0)
		return (false);
	if (strcmp(node->cmd.argv[0], "cd") == 0)
		return (true);
	else if (strcmp(node->cmd.argv[0], "pwd") == 0)
		return (true);
	else if (strcmp(node->cmd.argv[0], "echo") == 0)
		return (true);
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
		return (builtin_cd(msh, in, out, node));
	else if (strcmp(node->cmd.argv[0], "pwd") == 0)
		return (builtin_pwd(ac, node->cmd.argv, out, node));
	else if (strcmp(node->cmd.argv[0], "echo") == 0)
		return (builtin_echo(ac, node->cmd.argv, out, node));
	else if (strcmp(node->cmd.argv[0], "exit") == 0)
		return (builtin_exit(in, out, node));
	else if (strcmp(node->cmd.argv[0], "unset") == 0)
		return (builtin_unset(msh, ac, node->cmd.argv, node));
	else if (strcmp(node->cmd.argv[0], "env") == 0)
		return (builtin_env(msh, in, out, node));
	else if (strcmp(node->cmd.argv[0], "export") == 0)
		return (builtin_export(msh, in, out, node));
	return (0);
}
