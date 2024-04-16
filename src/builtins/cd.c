/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:33:06 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/16 13:08:30 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_outfile(t_node *node)
{
	int	flags;
	int	file;

	flags = O_WRONLY | O_CREAT;
	if (node->cmd.append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	file = open(node->cmd.outfile, flags, 0666);
	return (file);
}

static int	no_arg(t_minishell *msh)
{
	char	*home;
	char	*pwd;

	home = getourenv(msh, "HOME");
	if (!home)
		return (msh_builtin_error("cd", "HOME not set"), 1);
	pwd = getcwd(NULL, 0);
	setourenv(msh, "OLDPWD", pwd);
	chdir(home);
	pwd = getcwd(NULL, 0);
	setourenv(msh, "PWD", pwd);
	if (errno != 0)
		perror("cd");
	free(home);
	return (0);
}

static int	no_minus(t_minishell *msh, int file)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = getourenv(msh, "OLDPWD");
	pwd = getcwd(NULL, 0);
	chdir(oldpwd);
	setourenv(msh, "OLDPWD", pwd);
	pwd = getcwd(NULL, 0);
	setourenv(msh, "PWD", pwd);
	ft_putendl_fd(pwd, file);
	if (errno != 0)
		perror("cd");
	return (0);
}

static int	minus(t_minishell *msh, t_node *node)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	setourenv(msh, "OLDPWD", pwd);
	chdir(node->cmd.argv[1]);
	pwd = getcwd(NULL, 0);
	setourenv(msh, "PWD", pwd);
	if (errno != 0)
		perror("cd");
	return (0);
}

int	builtin_cd(t_minishell *msh, int parent_in, int parent_out, t_node *node)
{
	int		file;

	if (node->cmd.argc > 2)
		return (msh_builtin_error("msh", "too many arguments"), 1);
	if (parent_out != -1 || parent_in != -1)
		return (0);
	file = STDOUT_FILENO;
	if (node->cmd.outfile)
	{
		file = create_outfile(node);
		close (file);
	}
	if (node->cmd.argc == 1)
		no_arg(msh);
	else if (!strcmp(node->cmd.argv[1], "-"))
		no_minus(msh, file);
	else
		minus(msh, node);
	return (0);
}
