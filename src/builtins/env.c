/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:13:31 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/24 14:58:07 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_minishell *msh, int parent_in, int parent_out, t_node *node)
{
	int		i;
	int		file;
	int		exit_code;

	(void) parent_in;
	exit_code = 0;
	file = STDOUT_FILENO;
	if (parent_out != -1)
		file = parent_out;
	if (node->cmd.outfile)
		file = open_outfile(node);
	if (node->cmd.argc == 1)
	{
		i = -1;
		while (msh->env[++i])
			ft_putendl_fd(msh->env[i], file);
	}
	if (node->cmd.outfile)
		close(file);
	return (exit_code);
}
