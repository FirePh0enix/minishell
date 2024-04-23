/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 15:35:37 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/23 16:59:42 by vopekdas         ###   ########.fr       */
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
	free_env(msh);
	free_history(msh);
	ft_vector_free(msh->child_pids);
	close_fd_child(msh);
	rl_clear_history();
	msh_error_cmd(node->cmd.argv[0]);
	free_node_tree(node);
}

void __attribute__((noreturn))	free_at_end(t_minishell *msh, t_node *node)
{
	free_env(msh);
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
