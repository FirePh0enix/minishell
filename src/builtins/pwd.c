/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:37:05 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/22 16:23:58 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_minishell *msh, int parent_in, int parent_out, t_node *node)
{
	char	*s;
	int		file;

	(void) parent_in;
	s = getourenv(msh, "PWD");
	if (!s)
		s = "";
	file = 1;
	if (parent_out != -1)
		ft_putendl_fd(s, parent_out);
	else if (node->cmd.outfile)
	{
		file = open_outfile(node);
		if (file == -1)
			return (free(s), 1);
	}
	ft_putendl_fd(s, file);
	free(s);
	if (node->cmd.outfile)
		close(file);
	return (0);
}
