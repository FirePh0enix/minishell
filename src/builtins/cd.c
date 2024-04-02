/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:33:06 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/02 14:48:00 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <unistd.h>

int	builtin_cd(t_minishell *msh, int ac, char **av, int parent_out, t_node *node)
{
	char	*oldpwd;
	char	*pwd;
	int		flags;
	int		file;

	if (ac > 2)
		return (1);
	flags = O_WRONLY | O_CREAT;
	file = STDOUT_FILENO;
	if (parent_out != -1)
		file = parent_out;
	if (node->cmd.outfile)
	{
		if (node->cmd.append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		file = open(node->cmd.outfile, flags, 0666);
	}
	if (ac == 1)
	{
		char	*home = getourenv(msh, "HOME");
		if (!home)
			return (1);
		pwd = getcwd(NULL, 0);
		setourenv(msh, "OLDPWD", pwd);
		chdir(home);
		pwd = getcwd(NULL, 0);
		setourenv(msh, "PWD", pwd);
		if (errno != 0)
			perror("cd");
		free(home);
	}
	else if (!strcmp(av[1], "-"))
	{
		oldpwd = getourenv(msh, "OLDPWD");
		pwd = getcwd(NULL, 0);
		chdir(oldpwd);
		setourenv(msh, "OLDPWD", pwd);
		pwd = getcwd(NULL, 0);
		setourenv(msh, "PWD", pwd);
		ft_putendl_fd(pwd, file);
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
