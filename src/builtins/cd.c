/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:33:06 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/24 16:47:57 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	no_arg(t_minishell *msh)
{
	char	*home;
	char	*pwd;

	home = getourenv(msh, "HOME");
	if (!home)
		return (msh_builtin_error("cd", "HOME not set"), 1);
	pwd = getourenv(msh, "PWD");
	setourenv(msh, "OLDPWD", pwd);
	free(pwd);
	if (chdir(home) == -1)
	{
		msh_errno2("cd", home);
		free(home);
		return (1);
	}
	pwd = getcwd(NULL, 0);
	setourenv(msh, "PWD", pwd);
	free(pwd);
	free(home);
	return (0);
}

static int	minus(t_minishell *msh, int file)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = getourenv(msh, "OLDPWD");
	pwd = getourenv(msh, "PWD");
	if (chdir(oldpwd) == -1)
	{
		msh_errno2("cd", oldpwd);
		free(oldpwd);
		free(pwd);
		return (1);
	}
	setourenv(msh, "OLDPWD", pwd);
	free(pwd);
	pwd = getcwd(NULL, 0);
	setourenv(msh, "PWD", pwd);
	ft_putendl_fd(pwd, file);
	free(oldpwd);
	free(pwd);
	return (0);
}

static int	no_minus(t_minishell *msh, t_node *node)
{
	char	*pwd;
	char	*pwd2;
	char	*cdpath;
	char	*fs;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (ft_fprintf(2, CWDERA CWDERB, "chdir", strerror(errno)), 1);
	cdpath = getourenv(msh, "CDPATH");
	pwd2 = getourenv(msh, "PWD");
	setourenv(msh, "OLDPWD", pwd2);
	free(pwd2);
	free(pwd);
	if (cdpath && cdpath[0] != '\0')
		fs = ft_strjoin(cdpath, node->cmd.argv[1]);
	else
		fs = ft_strdup(node->cmd.argv[1]);
	if (chdir(fs) == -1)
	{
		msh_errno2("cd", fs);
		free(fs);
		return (1);
	}
	free(fs);
	pwd = getcwd(NULL, 0);
	setourenv(msh, "PWD", pwd);
	free(pwd);
	return (0);
}

void	close_outfile(int fd)
{
	if (fd != -1 && fd != STDOUT_FILENO)
		close(fd);
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
		file = open_outfile(node);
	if (node->cmd.argc == 1)
		return (close_outfile(file), no_arg(msh));
	else if (!ft_strcmp(node->cmd.argv[1], "-"))
		return (close_outfile(file), minus(msh, file));
	else
		return (close_outfile(file), no_minus(msh, node));
}
