/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:13:31 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/28 15:15:07 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_minishell *msh, int ac, char *av[])
{
	size_t	i;

	(void) ac;
	(void) av;
	i = 0;
	while (msh->env[i])
	{
		printf("%s\n", msh->env[i]);
		i++;
	}
	return (0);
}
