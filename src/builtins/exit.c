/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:02:43 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/15 15:04:51 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>

static bool	isnumeric(char *s)
{
	size_t	i;

	i = 0;
	while (isspace(s[i]))
		i++;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (false);
		i++;
	}
	return (true);
}

static int	exit_shell(char *s)
{
	long	i;

	if (!isnumeric(s))
		return (ft_fprintf(2, EXIT_ERRNUM, s), 2);
	i = ft_atoi(s);
	exit(i);
}

int	builtin_exit(int ac, char **av, int parent_in, int parent_out, t_node *node)
{
	int		flags;
	int		file;

	if (parent_out != -1 || parent_in != -1)
	{
		// FIXME: Check errors and other stuff
		if (ac == 2)
			return (ft_atoi(av[1]));
		else
			return (0);
	}
	flags = O_WRONLY | O_CREAT;
	file = 1;
	if (node->cmd.outfile)
	{
		if (node->cmd.append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file = open(node->cmd.outfile, flags, 0666);
		close(file);
	}
	ft_fprintf(1, "exit\n");
	if (ac == 2)
		return (exit_shell(av[1]));
	else if (ac == 1)
		return (exit_shell("0"));
	else
		return (msh_builtin_error("exit", "too many arguments"), -1);
}
