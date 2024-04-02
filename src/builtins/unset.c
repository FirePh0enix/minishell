/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:24:38 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/02 15:06:49 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_minishell *msh, int argc, char *argv[], t_node *node)
{
	size_t	size;
	size_t	i;
	int		flags;
	int		file;

	// TODO `unset env1 env2 env3` should unset all variables
	size = ft_strlen(argv[1]);
	i = 0;
	flags = O_WRONLY | O_CREAT;
	if (node->cmd.outfile)
	{
		if (node->cmd.append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file = open(node->cmd.outfile, flags, 0666);
		close(file);
	}
	while (msh->env[i])
	{
		if (ft_strlen(msh->env[i]) <= size)
		{
			i++;
			continue ;
		}
		if (!ft_strncmp(msh->env[i], argv[1], size) && msh->env[i][size] == '=')
		{
			ft_vector_del(&msh->env, i);
			break ;
		}
		i++;
	}
	return (0);
}
