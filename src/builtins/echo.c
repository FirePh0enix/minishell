/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:51:56 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/01 14:02:08 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	builtin_echo(int ac, char **av)
{
	int		i;
	bool	nl;

	if (!av || !*av)
		return (1);
	i = 1;
	nl = true;
	while (av[i] && ft_strlen(av[i]) >= 2 && strcmpn(av[i]) == 0)
	{
		i++;
		nl = false;
	}
	while (i < ac)
	{
		printf("%s", av[i]);
		if (i + 1 < ac)
			printf(" ");
		i++;
	}
	if (nl)
		printf("\n");
	return (0);
}
