/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:37:05 by vopekdas          #+#    #+#             */
/*   Updated: 2024/03/25 15:16:39 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(int ac, char **av)
{
	char	*s;

	if (ac != 1)
		return (printf("pwd: too many arguments\n"), 1);
	s = getcwd(NULL, 0);
	printf("%s\n", s);
	return (0);
}