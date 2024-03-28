/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:33:06 by vopekdas          #+#    #+#             */
/*   Updated: 2024/03/28 15:10:13 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_minishell *msh, int ac, char **av)
{
	char	*oldpwd;
	char	*pwd;

	if (ac != 2)
		return (1);
	if (!strcmp(av[1], "-"))
	{
		oldpwd = getourenv(msh, "OLDPWD");
		pwd = getcwd(NULL, 0);
		chdir(oldpwd);
		setourenv(msh, "OLDPWD", pwd);
		pwd = getcwd(NULL, 0);
		setourenv(msh, "PWD", pwd);
		printf("%s\n", pwd);
		if (errno != 0)
			perror("cd");
	}
	else
	{
		pwd = getcwd(NULL, 0);
		setourenv(msh, "OLDPWD", pwd);
		chdir(av[1]);
		pwd = getcwd(NULL, 0);
		setourenv(msh, "PWD", pwd);
		if (errno != 0)
			perror("cd");
	}
	return (0);
}
