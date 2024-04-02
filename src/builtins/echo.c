/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:51:56 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/02 15:13:32 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <minishell.h>

static int strcmpn(char *s)
{
	int	i;

	i = 0;
	if (s[i] != '-')
		return (1);
	i = 1;
	while (s[i])
	{
		if (s[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	builtin_echo(int ac, char **av, int parent_out, t_node *node)
{
	int		i;
	bool	nl;
	int		flags;
	int		file;

	if (!av || !*av)
		return (1);
	flags = O_WRONLY | O_CREAT;
	i = 1;
	nl = true;
	while (av[i] && ft_strlen(av[i]) >= 2 && strcmpn(av[i]) == 0)
	{
		i++;
		nl = false;
	}
	if (parent_out != -1)
	{
		while (i < ac)
		{
			ft_putstr_fd(av[i], parent_out);
			if (i + 1 < ac)
				ft_putstr_fd(" ", parent_out);
			i++;
		}
		if (nl)
			ft_putstr_fd("\n", parent_out);
	}
	else if (node->cmd.outfile)
	{
		if (node->cmd.append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file = open(node->cmd.outfile, flags, 0666);
		while (i < ac)
		{
			ft_putstr_fd(av[i], file);
			if (i + 1 < ac)
				ft_putstr_fd(" ", file);
			i++;
		}
		if (nl)
			ft_putstr_fd("\n", file);
		close(file);
	}
	else
	{
		while (i < ac)
		{
			printf("%s", av[i]);
			if (i + 1 < ac)
				printf(" ");
			i++;
		}
		if (nl)
			printf("\n");
	}
	return (0);
}
