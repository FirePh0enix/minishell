/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vopekdas <vopekdas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 23:45:53 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/17 16:25:18 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"
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
		if (!ft_vector_add(&minishell->env, &s))
			break ;
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
	if (strcmp(name, "*") == 0)
		return (ft_strdup(""));
	i = 0;
	while (i < ft_vector_size(minishell->env) - 1)
	{
		if (minishell->env[i] && ft_strlen(minishell->env[i]) > size
			&& ft_strncmp(name, minishell->env[i], size) == 0
			&& minishell->env[i][size] == '=')
			return (ft_strdup(ft_strchr(minishell->env[i], '=') + 1));
		i++;
	}
	if (!strcmp(name, "PATH"))
		return (ft_strdup("/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:"
			"/sbin:/bin"));
	return (NULL);
}

static int	try_replace_env(t_minishell *msh, char *name, char *value)
{
	const size_t	sz = ft_strlen(name);
	int				i;
	char			*s;
	char			*s2;

	i = -1;
	while (msh->env[++i])
	{
		if (ft_strlen(msh->env[i]) > sz && !ft_strncmp(msh->env[i], name, sz)
			&& msh->env[i][sz] == '=')
		{
			free(msh->env[i]);
			s = ft_strjoin(name, "=");
			if (!s)
				return (-1);
			s2 = ft_strjoin(s, value);
			if (!s2)
				return (free(s), -1);
			free(s);
			msh->env[i] = s2;
			return (-1);
		}
	}
	return (i);
}

void	setourenv(t_minishell *msh, char *name, char *value)
{
	int		i;
	char	*s;
	char	*s2;

	i = try_replace_env(msh, name, value);
	if (i == -1)
		return ;
	s = ft_strjoin(name, "=");
	if (!s)
		return ;
	s2 = ft_strjoin(s, value);
	if (!s2)
	{
		free(s);
		return ;
	}
	free(s);
	msh->env[i] = s2;
	s2 = NULL;
	ft_vector_add(&msh->env, &s2);
}

void	free_env(t_minishell *msh)
{
	ft_vector_deep_free(msh->env);
}
