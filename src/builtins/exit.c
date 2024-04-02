/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:02:43 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/02 14:54:03 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(int ac, char **av, t_node *node)
{
	int		exit_no;
	int		flags;
	int		file;

	exit_no = 0;
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
	if (ac == 2)
		exit_no = ft_atoi(av[1]);
	else if (ac != 1)
		return (-1);
	exit(exit_no);
	return (0);
}
