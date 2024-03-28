/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:45:53 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/28 15:05:45 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <string.h>
#include <stdio.h>

void	copy_env(t_minishell *minishell, char *envp[])
{
	char	*s;

	minishell->env = ft_vector(sizeof(char *), 0);
	while (*envp)
	{
		s = ft_strdup(*envp);
		if (!s)
			return ;
		ft_vector_add(&minishell->env, &s);
		envp++;
	}
	s = NULL;
	ft_vector_add(&minishell->env, &s);
}

char	*getourenv(t_minishell *minishell, char *name)
{
	const size_t	size = ft_strlen(name);
	size_t			i;

	if (strcmp(name, "?") == 0)
		return (ft_itoa(minishell->exit_code));
	i = 0;
	while (i < ft_vector_size(minishell->env) - 1)
	{
		if (minishell->env[i] && ft_strlen(minishell->env[i]) > size
				&& ft_strncmp(name, minishell->env[i], size) == 0
				&& minishell->env[i][size] == '=')
			return (ft_strdup(ft_strchr(minishell->env[i], '=') + 1));
		i++;
	}
	return (NULL);
}

void	setourenv(t_minishell *msh, char *name, char *value)
{
	const size_t	sz = ft_strlen(name);
	size_t			i;
	char			*s;
	char			*s2;

	i = 0;
	while (i < ft_vector_size(msh->env) - 1)
	{
		if (ft_strlen(msh->env[i]) > sz
			&& !ft_strncmp(msh->env[i], name, sz)
			&& msh->env[i][sz] == '=')
		{
			free(msh->env[i]);
			s = ft_strjoin(name, "=");
			s2 = ft_strjoin(s, value);
			free(s);
			msh->env[i] = s2;
			return ;
		}
		i++;
	}
	s = ft_strjoin(name, "=");
	s2 = ft_strjoin(s, value);
	free(s);
}
