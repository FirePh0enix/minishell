/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 15:35:37 by vopekdas          #+#    #+#             */
/*   Updated: 2024/06/11 02:55:36 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **s)
{
	size_t	i;

	i = 0;
	while (s[i])
		free(s[i++]);
	free(s);
}

void	free_when_no_cmd(t_minishell *msh, t_node *node)
{
	ft_vector_deep_free(msh->env);
	free_history(msh);
	ft_vector_free(msh->child_pids);
	close_fd_child(msh);
	rl_clear_history();
	if (msh->node_to_free)
	{
		free_node_tree(msh->node_to_free);
		msh->node_to_free = NULL;
	}
	free_node_tree(node);
}

void __attribute__((noreturn))	free_at_end(t_minishell *msh, t_node *node)
{
	ft_vector_deep_free(msh->env);
	free_history(msh);
	ft_vector_free(msh->child_pids);
	rl_clear_history();
	free_node_tree(node);
	exit(0);
}

void	exec_cmd_when_cmd_ok(t_minishell *msh, char *cmd, t_node *node)
{
	char	**av;

	av = node->cmd.argv;
	free_node_in_child(node);
	ft_exec_cmd(msh, cmd, av, msh->env);
}

char	*check_access(char *command)
{
	if (access(command, F_OK) != 0)
		return ((void *) 2);
	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	return ((void *) 1);
}
