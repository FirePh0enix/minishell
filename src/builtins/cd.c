/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:33:06 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/19 16:21:32 by vopekdas         ###   ########.fr       */
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

static int	minus(t_minishell *msh, int file)
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
	return (0);
}

static int	no_minus(t_minishell *msh, t_node *node)
{
	char	*pwd;
	char	*cdpath;
	char	*fs;

	pwd = getcwd(NULL, 0);
	cdpath = getourenv(msh, "CDPATH");
	setourenv(msh, "OLDPWD", pwd);
	free(pwd);
	if (cdpath && cdpath[0] != '\0')
		fs = ft_strjoin(cdpath, node->cmd.argv[1]);
	else
		fs = ft_strdup(node->cmd.argv[1]);
	chdir(fs);
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
