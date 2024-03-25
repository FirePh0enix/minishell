/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:02:43 by vopekdas          #+#    #+#             */
/*   Updated: 2024/03/25 15:06:34 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(int ac, char **av)
{
	int	exit_no;

	exit_no = 0;
	if (ac == 2)
		exit_no = ft_atoi(av[1]);
	else if (ac != 1)
		return (-1);
	exit(exit_no);
	return (0);
}
