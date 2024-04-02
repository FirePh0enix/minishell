/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:26:05 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/02 15:00:19 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(t_minishell *msh, int ac, char *av[], t_node *node)
{
	char	*arg;
	char	*eq;
	int		flags;
	int		file;

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
	arg = av[1];
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		// TODO "bash: export: `123': not a valid identifier" when there is not
		setourenv(msh, strndup(av[1], eq - av[1]), ft_strdup(eq + 1));
	}
	else
	{
		// FIXME This is completly broken, it does not export the variable
		setourenv(msh, arg, "");
	}
	return (0);
}
