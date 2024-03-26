/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:37:57 by vopekdas          #+#    #+#             */
/*   Updated: 2024/03/26 19:02:27 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <unistd.h>

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

int	exec_cmd(t_minishell *msh, t_node *node, int parent_in, int parent_out)
{
	int	pid;

	if (node->type == TY_CMD)
	{
		if (strcmp(node->cmd.argv[0], "cd") == 0)
			return (builtin_cd(node->cmd.argc, node->cmd.argv));
		else if (strcmp(node->cmd.argv[0], "pwd") == 0)
			return (builtin_pwd(node->cmd.argc, node->cmd.argv));
		else if (strcmp(node->cmd.argv[0], "echo") == 0)
			return (builtin_echo(node->cmd.argc, node->cmd.argv));
		else if (strcmp(node->cmd.argv[0], "exit") == 0)
			return (builtin_exit(node->cmd.argc, node->cmd.argv));
		pid = fork();
		if (pid == -1)
			return (-1);
		if (pid == 0)
		{
			if (parent_in != -1)
			{
				if (dup2(msh->pipe[0], STDIN_FILENO) == -1)
					return (printf("ERROR DUP2\n"));
				close(msh->pipe[0]);
			}
			if (parent_out != -1)
			{
				if (dup2(msh->pipe[1], STDOUT_FILENO) == -1)
					return (printf("ERROR DUP2\n"));
				close(msh->pipe[1]);
			}
			if (ft_exec_cmd(node->cmd.argv, msh->env) == -1)
				return (printf("ERROR EXECVE\n"));
		}
		else
		{
			while (wait(NULL) > 0)
			{
				printf("WAITING\n");
				if (g_signum == SIGINT)
				{
					kill(pid, SIGINT);
					g_signum = -1;
				}
			}
		}
	}
	else if (node->type == TY_PIPE)
	{
		exec_cmd(msh, node->pipe.left, parent_in, msh->pipe[0]);
		while(wait(NULL) > 0)
			;
		exec_cmd(msh, node->pipe.right, msh->pipe[1], parent_out);
	}
	return (0);
}
