/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:37:05 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/18 15:22:21 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_minishell *msh, int parent_in, int parent_out, t_node *node)
{
	char	*s;
	int		flags;
	int		file;

	flags = O_WRONLY | O_CREAT;
	s = getourenv(msh, "PWD");
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
