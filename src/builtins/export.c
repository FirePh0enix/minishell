/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:26:05 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/28 15:50:41 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(t_minishell *msh, int ac, char *av[])
{
	char	*arg;
	char	*eq;

	arg = av[1];
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		// TODO Code this thing!
		setourenv(msh, strndup(av[1], eq - av[1]), ft_strdup(eq + 1));
	}
	else
	{
		setourenv(msh, arg, "=");
	}
	return (0);
}
