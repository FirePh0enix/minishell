/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:37:57 by vopekdas          #+#    #+#             */
/*   Updated: 2024/03/25 14:20:02 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdio.h>

char	**ft_get_path(char **env)
{
	char	**path;

	if (!env || !*env)
		return (NULL);
	path = NULL;
	while (*env)
	{
		if (ft_strncmp(*env, "PATH=", 5) == 0)
		{
			path = ft_split(*env + 5, ':');
			if (!path)
				return (NULL);
			return (path);
		}
		env++;
	}
	return (NULL);
}

char	*ft_create_path(char *command, char **envp)
{
	char	**path;
	int		i;
	char	buf[256];

	if (!envp || !command || !*envp || !*command)
		return (NULL);
	i = 0;
	path = ft_get_path(envp);
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

int	ft_exec_cmd(char **av, char **envp)
{
	char	*path;

	path = ft_create_path(av[0], envp);
	if (!path)
		return (-1);
	if (execve(path, av, envp) == -1)
	{
		free(path);
		return (-1);
	}
	free(path);
	return (0);
}

int	exec_cmd(t_minishell *msh, t_node *node)
{
	int	pid;

	if (node->type == TY_CMD)
	{
		pid = fork();
		if (pid == -1)
			return (-1);
		if (pid == 0)
		{
			if (ft_exec_cmd(node->cmd.argv, msh->env) == -1)
				return (-1);
		}
		else
		{
			while (wait(NULL) > 0)
			{
				if (g_signum == SIGINT)
				{
					kill(pid, SIGINT);
					g_signum = -1;
				}
			}
		}
	}
	return (0);
}