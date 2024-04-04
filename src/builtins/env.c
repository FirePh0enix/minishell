/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:13:31 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/04 13:14:15 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_minishell *msh, int parent_out, t_node *node)
{
	int	i;
	int	flags;
	int	file;

	file = STDOUT_FILENO;
	flags = O_WRONLY | O_CREAT;
	if (parent_out != -1)
		file = parent_out;
	if (node->cmd.outfile)
	{
		if (node->cmd.append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file = open(node->cmd.outfile, flags, 0666);
		if (file == -1)
			return (msh_error("unable to open outfile"), -1);
	}
	i = -1;
	while (msh->env[++i])
		ft_putendl_fd(msh->env[i], file);
	if (node->cmd.outfile)
		close(file);
	return (0);
}
