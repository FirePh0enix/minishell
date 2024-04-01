/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 13:30:43 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/01 13:30:58 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

char	**ft_get_path(t_minishell *msh)
{
	char	**path;
	char	*env;

	env = getourenv(msh, "PATH");
	if (env)
	{
		path = ft_split(env, ':');
		if (!path)
			return (NULL);
		return (path);
	}
	return (NULL);
}

char	*ft_create_path(t_minishell *msh, char *command)
{
	char	**path;
	int		i;
	char	buf[256];

	if (!command || !*command)
		return (NULL);
	if ((ft_strlen(command) >= 2 && !ft_strncmp(command, "./", 2))
		|| command[0] == '/')
		return (ft_strdup(command));
	i = 0;
	path = ft_get_path(msh);
	if (!path)
		return (NULL);
	while (path[i])
	{
		ft_sprintf(buf, "%s/%s", path[i], command);
		if (access(buf, X_OK) == 0)
			return (ft_strdup(buf));
		i++;
	}
	return (NULL);
}

int	ft_exec_cmd(char *cmd, char **av, char **envp)
{
	if (execve(cmd, av, envp) == -1)
	{
		free(cmd);
		return (-1);
	}
	free(cmd);
	return (0);
}

int	copy_file(int out, int in)
{
	char	buf[4096];
	int		n;

	n = 4096;
	while (n == 4096)
	{
		n = read(in, buf, 4096);
		write(out, buf, n);
	}
	return (0);
}
