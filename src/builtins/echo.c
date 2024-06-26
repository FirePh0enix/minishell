/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:51:56 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/19 14:44:45 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	strcmpn(char *s)
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

static void	print(char **av, int i, int fd, bool nl)
{
	while (av[i])
	{
		ft_putstr_fd(av[i], fd);
		if (av[i + 1])
			ft_putstr_fd(" ", fd);
		i++;
	}
	if (nl)
		ft_putstr_fd("\n", fd);
}

int	builtin_echo(int ac, char **av, int parent_out, t_node *node)
{
	int		i;
	bool	nl;
	int		file;

	(void) ac;
	i = 1;
	nl = true;
	file = STDOUT_FILENO;
	while (av[i] && ft_strlen(av[i]) >= 2 && strcmpn(av[i]) == 0)
	{
		i++;
		nl = false;
	}
	if (parent_out != -1)
		file = parent_out;
	if (node->cmd.outfile)
		file = open_outfile(node);
	if (file == -1)
		return (-1);
	print(av, i, file, nl);
	if (node->cmd.outfile)
		close(file);
	return (0);
}
