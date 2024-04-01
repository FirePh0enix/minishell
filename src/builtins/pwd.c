/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:37:05 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/01 14:18:03 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(int ac, char **av, int parent_out, t_node *node)
{
	char	*s;
	int		flags;
	int		file;

	(void) av;
	flags = O_WRONLY | O_CREAT;
	if (ac != 1)
		return (printf("pwd: too many arguments\n"), 1);
	s = getcwd(NULL, 0);
	if (parent_out != -1)
	{
		ft_putstr_fd(s, parent_out);
		ft_putstr_fd("\n", parent_out);
	}
	else if (node->cmd.outfile)
	{
		if (node->cmd.append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file = open(node->cmd.outfile, flags, 0666);
		ft_putstr_fd(s, file);
		ft_putstr_fd("\n", file);
	}
	else
		printf("%s\n", s);
	return (0);
}
