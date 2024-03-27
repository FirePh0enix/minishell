/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 13:37:57 by vopekdas          #+#    #+#             */
/*   Updated: 2024/03/27 16:39:46 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
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

	if (ft_strncmp(av[0], "./", 2) == 0)
		path = ft_strdup(av[0]);
	else
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

int    exec_cmd(t_minishell *msh, t_node *node, int parent_in, int parent_out)
{
	int	pid;
	int	file;
	int	tmp;
	int	fd[2];

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
			if (parent_in != -1 && dup2(parent_in, STDIN_FILENO) == -1)
				return (printf("ERROR DUP2 PARENT_IN\n"), 1);
			if (node->cmd.infile)
			{
				file = open(node->cmd.infile, O_RDONLY);
				if (dup2(file, STDIN_FILENO) == -1)
					return (printf("ERROR DUP2 TMP\n"), 1);
			}
			if (parent_out != -1)
			{
				if (dup2(parent_out, STDOUT_FILENO) == -1)
					return (printf("ERROR DUP2 PARENT_OUT\n"), 1);
			}
			if (node->cmd.outfile)
			{
				int flags = O_WRONLY | O_CREAT;
				if (node->cmd.append)
					flags |= O_APPEND;
				else
					flags |= O_TRUNC;
				file = open(node->cmd.outfile, flags, 0666);
				if (dup2(file, STDOUT_FILENO) == -1)
					return (printf("ERROR OUTFILE\n"), 1);
			}
			if (ft_exec_cmd(node->cmd.argv, msh->env) == -1)
				return (printf("ERROR EXECVE\n"), 1);
		}
		else
		{
			if (parent_in != -1)
				close(parent_in);
			if (parent_out != -1)
				close(parent_out);
			while (wait(NULL) > 0)
			{
				if (g_signum != -1)
					kill(pid, g_signum);
			}
		}
	}
	else if (node->type == TY_PIPE)
	{
		pipe(fd);
		exec_cmd(msh, node->pipe.left, parent_in, fd[1]);
		close(fd[1]);
		exec_cmd(msh, node->pipe.right, fd[0], parent_out);
		close(fd[0]);
	}
	return (0);
}
