/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:51:56 by vopekdas          #+#    #+#             */
/*   Updated: 2024/03/25 15:02:49 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	builtin_echo(int ac, char **av)
{
	int		i;
	bool	nl;

	i = 1;
	nl = true;
	if (strcmp(av[1], "-n") == 0)
	{
		i = 2;
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