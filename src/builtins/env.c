/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:13:31 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/01 14:17:07 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_minishell *msh, int parent_out, t_node *node)
{
	size_t	i;
	int		flags;
	int		file;

	i = 0;
	flags = O_WRONLY | O_CREAT;
	if (parent_out != -1)
	{
		while (msh->env[i])
		{
			ft_putstr_fd(msh->env[i], parent_out);
			ft_putstr_fd("\n", parent_out);
			i++;
		}
	}
	else if (node->cmd.outfile)
	{
		if (node->cmd.append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file = open(node->cmd.outfile, flags, 0666);
		while (msh->env[i])
		{
			ft_putstr_fd(msh->env[i], file);
			ft_putstr_fd("\n", file);
			i++;
		}
	}
	else
	{
		while (msh->env[i])
		{
			printf("%s\n", msh->env[i]);
			i++;
		}
	}
	return (0);
}
