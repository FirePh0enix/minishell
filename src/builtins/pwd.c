/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:37:05 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/23 15:30:37 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_minishell *msh, int parent_in, int parent_out, t_node *node)
{
	char	*s;
	int		file;

	(void) msh;
	(void) parent_in;
	s = getcwd(NULL, 0);
	if (!s)
		return (ft_fprintf(2, CWDERA CWDERB, "pwd", strerror(errno)), 1);
	file = 1;
	if (parent_out != -1)
		return (ft_putendl_fd(s, parent_out), free(s), 0);
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
