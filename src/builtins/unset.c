/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:24:38 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/27 16:53:28 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_minishell *msh, int argc, char *argv[])
{
	size_t	size;
	size_t	i;

	if (argc != 2)
		return (2);
	size = ft_strlen(argv[1]);
	i = 0;
	while (msh->env[i])
	{
		if (ft_strlen(msh->env[i]) > size)
		{
			i++;
			continue ;
		}
		if (ft_strncmp(msh->env[i], argv[1], size) && msh->env[i][size] == '=')
		{
			ft_vector_del(&msh->env, i);
			break ;
		}
		i++;
	}
	return (0);
}
