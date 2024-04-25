/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 22:20:11 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/25 11:42:44 by vopekdas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"

static char	**dup_envs(char **old_env)
{
	char		**env;
	size_t		i;

	env = ft_vector(sizeof(char *), ft_vector_size(old_env));
	if (!env)
		return (NULL);
	i = 0;
	while (i < ft_vector_size(old_env) - 1)
	{
		env[i] = ft_strdup(old_env[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}

t_minishell	*dup_msh(t_minishell *msh)
{
	t_minishell	*new_msh;

	new_msh = ft_calloc(1, sizeof(t_minishell));
	if (!new_msh)
		return (NULL);
	new_msh->child_pids = ft_vector(sizeof(pid_t), 0);
	if (!new_msh->child_pids)
		return (free(new_msh), NULL);
	new_msh->open_fds = ft_vector(sizeof(pid_t), 0);
	if (!new_msh->open_fds)
		return (free(new_msh), ft_vector_free(new_msh->child_pids), NULL);
	new_msh->env = dup_envs(msh->env);
	if (!new_msh->env)
		return (ft_vector_free(new_msh->child_pids), free(new_msh), NULL);
	new_msh->init_path = getourenv(msh, "PWD");
	return (new_msh);
}

void	restore_msh(t_minishell *msh)
{
	size_t	i;

	if (msh->init_path)
	{
		chdir(msh->init_path);
		free(msh->init_path);
	}
	ft_vector_free(msh->child_pids);
	ft_vector_free(msh->open_fds);
	i = 0;
	while (msh->env[i])
	{
		free(msh->env[i]);
		i++;
	}
	ft_vector_free(msh->env);
	free(msh);
}
