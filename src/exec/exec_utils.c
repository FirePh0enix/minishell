/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 13:30:43 by vopekdas          #+#    #+#             */
/*   Updated: 2024/04/24 15:30:03 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

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
		free(env);
		return (path);
	}
	return (NULL);
}

char	*ft_create_path(t_minishell *msh, char *command)
{
	char	**path;
	int		i;
	char	buf[512];

	if (!command || !*command)
		return (NULL);
	if ((ft_strlen(command) >= 2 && !ft_strncmp(command, "./", 2))
		|| command[0] == '/')
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	i = 0;
	path = ft_get_path(msh);
	if (!path)
		return (command);
	while (path[i])
	{
		ft_sprintf(buf, "%s/%s", path[i], command);
		if (access(buf, X_OK) == 0)
		{
			free_split(path);
			return (ft_strdup(buf));
		}
		i++;
	}
	free_split(path);
	return (NULL);
}

static void	free_in_exec(t_minishell *msh, char *cmd, char **av)
{
	free_env(msh);
	free_history(msh);
	ft_vector_free(msh->child_pids);
	ft_vector_deep_free(av);
	(void) cmd;
}

int	ft_exec_cmd(t_minishell *msh, char *cmd, char **av, char **envp)
{
	int	fd;

	fd = open(cmd, O_DIRECTORY | O_RDONLY);
	if (fd != -1)
	{
		close(fd);
		errno = EISDIR;
		msh_errno(cmd);
		free_in_exec(msh, cmd, av);
		free(cmd);
		exit(126);
	}
	errno = 0;
	if (execve(cmd, av, envp) == -1)
	{
		msh_errno(cmd);
		free_in_exec(msh, cmd, av);
		exit(code_for_errno());
	}
	free(cmd);
	return (0);
}

int	code_for_errno(void)
{
	if (errno == EACCES)
		return (126);
	else if (errno == ENOENT)
		return (127);
	return (-1);
}
