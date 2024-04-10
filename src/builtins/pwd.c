/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:37:05 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/10 11:29:30 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(int ac, char **av, int parent_out, t_node *node)
{
	char	*s;
	int		flags;
	int		file;

	(void) ac;
	(void) av;
	flags = O_WRONLY | O_CREAT;
	s = getcwd(NULL, 0);
	if (parent_out != -1)
		ft_putendl_fd(s, parent_out);
	else if (node->cmd.outfile)
	{
		if (node->cmd.append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file = open(node->cmd.outfile, flags, 0666);
		ft_putendl_fd(s, file);
		close(file);
	}
	else
		printf("%s\n", s);
	return (0);
}
