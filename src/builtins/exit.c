/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:02:43 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/10 14:07:40 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(int ac, char **av, int parent_in, int parent_out, t_node *node)
{
	int		exit_no;
	int		flags;
	int		file;

	if (parent_out != -1 || parent_in != -1)
		return (0);
	exit_no = 0;
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
	ft_fprintf(file, "exit\n");
	if (ac == 2)
		exit_no = ft_atoi(av[1]);
	else if (ac != 1)
		return (msh_builtin_error("exit", "too many arguments"), -1);
	exit(exit_no);
	return (0);
}
