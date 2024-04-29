/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:13:31 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/29 11:24:09 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_var(char *s, int fd)
{
	char	*s2;

	s2 = ft_strchr(s, '=');
	if (!s2 || *(s2 + 1) == '\0')
		return ;
	ft_putendl_fd(s, fd);
}

int	builtin_env(t_minishell *msh, int parent_in, int parent_out, t_node *node)
{
	int		i;
	int		file;
	int		exit_code;

	(void) parent_in;
	exit_code = 0;
	file = STDOUT_FILENO;
	if (parent_out != -1)
		file = parent_out;
	if (node->cmd.outfile)
		file = open_outfile(node);
	if (node->cmd.argc == 1)
	{
		i = -1;
		while (msh->env[++i])
			print_var(msh->env[i], file);
	}
	if (node->cmd.outfile)
		close(file);
	return (exit_code);
}
