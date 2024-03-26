/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:51:56 by vopekdas          #+#    #+#             */
/*   Updated: 2024/03/26 11:52:24 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	builtin_echo(int ac, char **av)
{
	int		i;
	bool	nl;

	if (!av || !*av)
		return 1;
	i = 1;
	nl = true;
	while (av[i] && strcmp(av[i], "-n") == 0)
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
